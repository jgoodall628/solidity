/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
// SPDX-License-Identifier: GPL-3.0
/**
 * Container of (unparsed) Yul functions identified by name which are meant to be generated
 * only once.
 */

#include <libsolidity/codegen/MultiUseYulFunctionCollector.h>

#include <liblangutil/Exceptions.h>
#include <libsolutil/Whiskers.h>
#include <libsolutil/StringUtils.h>

#include <regex>

using namespace std;
using namespace solidity;
using namespace solidity::frontend;
using namespace solidity::util;

string MultiUseYulFunctionCollector::requestedFunctions(string const& _sourceLocationComment)
{
	string result;
	// std::map guarantees ascending order when iterating through its keys.
	for (auto const& [name, code]: m_requestedFunctions)
	{
		solAssert(code != "<<STUB<<", "");

		// Insert source location comment if it doesn't exist already.
		if (code.find("/// @src") == string::npos && !_sourceLocationComment.empty())
		{
			smatch match;
			regex functionRegex("function[ \t]+[a-zA-Z0-9_$]+\\([^\\)]*\\)");
			solAssert(regex_search(code, match, functionRegex), "");

			size_t functionStart = (size_t) match.position();
			size_t lineStart = code.substr(0, functionStart).find_last_of('\n');
			size_t numTabs = lineStart == string::npos ? 0 : functionStart - lineStart - 1;
			result += code.substr(0, functionStart) +
				_sourceLocationComment +
				"\n" +
				string(numTabs, '\t') +
				code.substr(functionStart, string::npos);
		}
		else
			result += code;
	}
	m_requestedFunctions.clear();
	return result;
}

string MultiUseYulFunctionCollector::createFunction(string const& _name, function<string ()> const& _creator)
{
	if (!m_requestedFunctions.count(_name))
	{
		m_requestedFunctions[_name] = "<<STUB<<";
		string fun = _creator();
		solAssert(!fun.empty(), "");
		solAssert(fun.find("function " + _name + "(") != string::npos, "Function not properly named.");
		m_requestedFunctions[_name] = std::move(fun);
	}
	return _name;
}

string MultiUseYulFunctionCollector::createFunction(
	string const& _name,
	function<string(vector<string>&, vector<string>&)> const& _creator
)
{
	solAssert(!_name.empty(), "");
	if (!m_requestedFunctions.count(_name))
	{
		m_requestedFunctions[_name] = "<<STUB<<";
		vector<string> arguments;
		vector<string> returnParameters;
		string body = _creator(arguments, returnParameters);
		solAssert(!body.empty(), "");

		m_requestedFunctions[_name] = Whiskers(R"(
			function <functionName>(<args>)<?+retParams> -> <retParams></+retParams> {
				<body>
			}
		)")
		("functionName", _name)
		("args", joinHumanReadable(arguments))
		("retParams", joinHumanReadable(returnParameters))
		("body", body)
		.render();;
	}
	return _name;
}
