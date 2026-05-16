# Описание классов и модулей

Краткий обзор проекта по слоям: **Core** (мир и цикл симуляции), **Features** (юниты и действия), **IO** (сценарий и лог).  
Подробная архитектура — в `SOLUTION_PROPOSAL.md`, спорные места — в `KNOWN_ISSUES.md`.

---

## Предлагаемая архитектура

```
┌─────────┐     ┌──────────┐     ┌─────────────────────────────────┐
│   IO    │     │ Features │     │             Core                │
│ EventLog│◄────│ actions  │────►│ World, Simulation, ActionSession│
│ Events  │     │ rules    │     │ IUnit, IBehavior                │
│ Parser  │     │ factories│     └─────────────────────────────────┘
└─────────┘     └──────────┘
```

- `Core` — мир, жизненный цикл, `IUnit`, чтение состояния в ходе.
- `Features` — интерфейсы способностей, юниты, поведение, правила, мутации мира, вызов лога через IO.
- `IO` — формат команд/событий, парсер, вывод в stdout. **Новое событие = новый файл в `IO/Events`.**

---

## Core

База, которую не нужно менять при добавлении новых юнитов или событий.

### Типы и данные

| Имя | Что это | Зачем |
|-----|---------|--------|
| `UnitId` | Обёртка над `uint32_t` | Уникальный id юнита из сценария |
| `Position` | `x`, `y` | Простые координаты на карте; без правил движения и дистанций |
| `UnitState` | `id`, `pos`, `spawnOrder`, `pendingRemoval` | Минимум состояния, который знает Core: где юнит, порядок хода, помечен ли на удаление |
| `StatBlock` | `unordered_map<string, int>` | Характеристики без фиксированного списка полей в Core |
| `IUnit` | Минимальный интерфейс юнита: id, позиция, порядок, тип, stats, pending removal, `occupiesCell`, `act` | Общий контракт для `World`; конкретные юниты живут в Features |

**Почему так:** Core не хранит список указателей на способности. HP, марш и конкретная логика живут в классах юнитов из Features, а способности проверяются через `dynamic_cast`.

---

### Интерфейсы способностей

Capability-интерфейсы находятся в `src/Features/Capabilities/`, а не в `Core`.

| Интерфейс | Назначение | Кто реализует (сейчас / план) |
|-----------|------------|-------------------------------|
| `IDamageable` | `changeHp`, `isDead`, `currentHp` | Мечник, охотник |
| `IMovable` | Физическая возможность сделать шаг; задаёт `maxStepDistance()` | Мечник, охотник |
| `IMarchable` | Цель марша: set/get/clear | Мечник, охотник |
| `IBehavior` | Интерфейс для переиспользуемой цепочки правил `BehaviorSequence` | Внутри конкретных юнитов |

**Почему интерфейсы, а не один жирный класс:** у башни нет марша, у мины нет HP — код проверяет capability через `dynamic_cast`, не через тип юнита и не через поля в `Unit`.

---

### `World`

Хранит карту (`width`/`height`), все `std::unique_ptr<IUnit>`, индекс занятых клеток `occupied_`.

- `createMap`, `spawnUnit`, `removeUnit`
- `relocateUnit` — технически переставляет юнита и синхронизирует `occupied_`
- `getUnit`, `isInside`, `isCellOccupied`, `cellOccupant`
- `unitOrder` — порядок ходов по `spawnOrder`

**Почему `occupied_` отдельно:** быстрый ответ «клетка свободна?»; в индекс попадают только юниты, у которых `IUnit::occupiesCell()` возвращает `true`. Будущие `Crow` и `Mine` смогут вернуть `false`, не добавляя новый capability-интерфейс в Core.

`setCellOccupied` и `clearCell` — private-helpers `World`. Внешний код не трогает индекс напрямую: игровое движение идёт через `features::actions::moveUnit`, а он после проверок вызывает `World::relocateUnit`.

`World` не считает Chebyshev-дистанции и не ищет цели по радиусу. Это логика сетки и боя, она находится в `Features/Common/Navigation`.

---

### `ActionSession`

Снимок доступа к миру **на один ход**: запросы (`getPosition`, `getStats`, `getUnit`, `isCellOccupied`, `isInside`) и `world()` + `tick()`.

**Не содержит:** урона, движения, логов.  
**Почему:** Core даёт только чтение; мутации и события — в Features (`actions`, `io::events`).

---

### `Simulation`

Пошаговый цикл: команды из сценария выполняются на тике 1, а ходы юнитов начинаются с тика 2.

- `createMap` / `spawnUnit` — только мир, **без лога**
- `run(events)` — цикл: `removeDeadUnits` → ходы по `unitOrder` → `unit.act`
- `eventTick()` — номер тика для лога при командах сценария

Команда `MARCH` обрабатывается в `Features/SimulationCommands`: там проверяется `IMarchable`, выставляется цель и пишется `MARCH_STARTED`. `Simulation` не знает о марше.

**Почему лог снаружи:** новые события добавляются в IO, Core не меняется.

---

## Features

Игровая логика: юниты, правила хода, действия над миром.

### Общее

| Имя | Что делает | Почему |
|-----|------------|--------|
| `stats` (`Features/Stats.hpp`) | Ключи `"strength"`, `"agility"`, … и `get(stats, key)` | Одно место для имён статов; Core не знает ключей |
| `IActionRule` | Одно правило за ход: `execute` → true, если действие выполнено | Разбивка поведения на шаги (удар → ход) |
| `BehaviorSequence` | Цепочка правил; первое успешное завершает ход | Используется внутри `Swordsman` и `Hunter` |
| `MeleeAttackRule` | Удар по первому соседу, урон из стата | |
| `RangedAttackRule` | Выстрел в диапазоне; опция «не стрелять, если есть соседи» | Охотник |
| `MoveToTargetRule` | Шаг к цели марша или конец марша | Если шаг пришёл ровно в цель, сразу пишет `MARCH_ENDED` на том же тике после `UNIT_MOVED` |
| `actions` | `dealDamage`, `moveUnit`, `endMarch`, `healUnit`, `scheduleRemoval` | Меняют `World` и пишут в `EventEmitter` через `io::events`; `moveUnit` проверяет `IMovable::maxStepDistance()` и вызывает `World::relocateUnit` |
| `navigation` | `chebyshevDistance`, `isAdjacent`, `unitsInRange`, `bestStepToward` | Геометрия сетки, поиск целей и стратегия движения не находятся в Core |
| `SimulationCommands` | `createMap` / `spawnUnit` / `startMarch` + лог на тике 1 | Симуляция не зависит от IO-формата событий |
| `ScenarioCommandRegistration` / `registerScenarioCommands` | Привязка команд файла к `Simulation` + `EventEmitter` | Вся регистрация сценария в одном адаптере между IO и Features |

---

### Мечник (`swordsman`)

| Класс | Роль |
|-------|------|
| `Swordsman` | Полноценный юнит: `IUnit`, `features::IDamageable`, `features::IMovable`, `features::IMarchable`; HP, марш, шаг 1 клетка, `BehaviorSequence`: удар → движение |
| `SwordsmanFactory::make` | Возвращает `std::unique_ptr<IUnit>` с `strength` |

---

### Охотник (`hunter`)

| Класс | Роль |
|-------|------|
| `Hunter` | Полноценный юнит: `IUnit`, `features::IDamageable`, `features::IMovable`, `features::IMarchable`; HP, марш, шаг 1 клетка, цепочка: выстрел → удар → движение |
| `HunterFactory::make` | Статы `agility`, `strength`, `range` |

**Почему отдельные папки:** новые юниты добавляют файлы только в `Features/`, Core не трогают.

---

## IO

Ввод сценария и вывод событий в **stdout** (формат из задания).

### События (`IO/Events/*`)

Структуры вроде `MapCreated`, `UnitSpawned`, `UnitAttacked` — поля + `Name` + `visit()` для печати.

**Новое событие:** новый `.hpp` в `Events/`, вызов `emitter.log(tick, event)`.

---

### Команды (`IO/Commands/*`)

`CreateMap`, `SpawnSwordsman`, `SpawnHunter`, `March` — поля для парсера, `Name`, `visit()`.

---

### Система

| Класс | Что делает | Почему |
|-------|------------|--------|
| `EventLog` | `log(tick, event)` → `[tick] EVENT_NAME field=value ...` | Единый формат вывода, как в примере `main.cpp` |
| `EventEmitter` | Обёртка: `log(tick, io::Event)` | Расширяемый API без виртуальных методов на каждое событие |
| `io::events` | Хелперы: Core-типы → `io::Events` (координаты, `UnitId`) | Features не собирают строки событий вручную |
| `CommandParser` | Регистрация обработчиков по имени команды, `parse(stream)` | Готовый парсер из задания |
| `PrintFieldVisitor` | Печать `name=value` для команд и событий | Общий механизм с `visit()` |
| `printDebug` | Вывод команды в отладочном виде | Использовался в примере `main.cpp` |

`CommandParserVisitor` — деталь парсера, читает поля из строки сценария.

---

## main

1. Открывает файл сценария.  
2. `EventLog` + `EventEmitter`, `World`, `Simulation`.  
3. `registerScenarioCommands` → `parser.parse` (тик 1).  
4. `simulation.run(events)` (тики 2+).

**Почему тонкий main:** связывает слои, не знает деталей ударов и формата полей событий.

---

## Поток одного тика (кратко)

```
Simulation::tick
  → removeDeadUnits (pendingRemoval)
  → для каждого id по spawnOrder:
       ActionSession(session)
       unit.act → rules → actions + io::events
```

---

## Связанные файлы

| Файл | Содержание |
|------|------------|
| `KNOWN_ISSUES.md` | Компромиссы и заметки по реализации |
| `README.md` | Условие задания |
| `src/Core/README.md`, `Features/README.md`, `IO/README.md` | Роль каталогов |
