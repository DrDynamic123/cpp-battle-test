#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

namespace sw
{
	class CommandParserVisitor
	{
	private:
		std::istream& _stream;

	public:
		CommandParserVisitor(std::istream& stream) :
				_stream(stream)
		{}

		template <class TField>
		void visit(const char* name, TField& field)
		{
			if (!(_stream >> field))
			{
				throw std::runtime_error("Failed to parse command field: " + std::string(name));
			}
		}
	};
}
