/* -----------------------------------------------------------------------------
libconsole - Command parser
Copyright (C) 2006  Joachim Klahr

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
------------------------------------------------------------------------------*/
#include "console.h"
#include <iostream>

#ifndef NULL
#	define NULL 0
#endif

namespace Console
{
	Manager* Manager::mSingleton = NULL;

	// -------------------------------------------------------------------------
	Manager* Manager::singleton()
	{
		if(mSingleton == NULL)
			mSingleton = new Manager();

		return mSingleton;
	}

	// -------------------------------------------------------------------------
	void Manager::clear()
	{
		delete mSingleton;

		mSingleton = NULL;
	}

	// -------------------------------------------------------------------------
	void Manager::executeCommand(const std::string &command)
	{
		std::vector<std::string> parsed = parse(command);

		switch(parsed.size())
		{
			case 3:
			{
				std::vector<Ref1i>::iterator i;
				for(i = mRef1is.begin(); i != mRef1is.end(); ++i)
					if(i->name == parsed[0] && i->function == parsed[1])
					{
						i->functionPtr(i->object, atoi(parsed[2].c_str()));

						return;
					}

				std::vector<Ref1f>::iterator j;
				for(j = mRef1fs.begin(); j != mRef1fs.end(); ++j)
					if(j->name == parsed[0] && j->function == parsed[1])
					{
						j->functionPtr(j->object, static_cast<float>(atof(parsed[2].c_str())));

						return;
					}
			}
			break;
			default:
				break;
		}
	}

	// -------------------------------------------------------------------------
	void Manager::addRef1i(const std::string &name, const std::string &function,
		                   void* object, void (*functionPtr)(void* object, int param))
	{
		Ref1i ref;

		ref.name        = name;
		ref.function    = function;
		ref.object      = object;
		ref.functionPtr = functionPtr;

		mRef1is.push_back(ref);
	}

	// -------------------------------------------------------------------------
	void Manager::addRef1f(const std::string &name, const std::string &function,
						   void* object, void (*functionPtr)(void* object, float param))
	{
		Ref1f ref;

		ref.name        = name;
		ref.function    = function;
		ref.object      = object;
		ref.functionPtr = functionPtr;

		mRef1fs.push_back(ref);
	}

	// -------------------------------------------------------------------------
	void Manager::removeRef1i(const std::string &name, const std::string &function)
	{
		std::vector<Ref1i>::iterator i;
		for(i = mRef1is.begin(); i != mRef1is.end(); ++i)
			if(i->name == name && i->function == function)
			{
				mRef1is.erase(i);

				return;
			}
	}

	// -------------------------------------------------------------------------
	void Manager::removeRef1f(const std::string &name, const std::string &function)
	{
		std::vector<Ref1f>::iterator i;
		for(i = mRef1fs.begin(); i != mRef1fs.end(); ++i)
			if(i->name == name && i->function == function)
			{
				mRef1fs.erase(i);

				return;
			}
	}

	// -------------------------------------------------------------------------
	Manager::Manager()
	{
	}

	// -------------------------------------------------------------------------
	Manager::~Manager()
	{
	}

	// -------------------------------------------------------------------------
	std::vector<std::string> Manager::parse(const std::string &command)
	{
		std::string part;
		std::vector<std::string> parsed;

		for(unsigned int i = 0; i < command.size(); ++i)
		{
			if(command[i] == 0x20)
			{
				parsed.push_back(part);
				part.clear();
				continue;
			}

			part.push_back(command[i]);
		}

		if(command.size())
			parsed.push_back(part);

		return parsed;
	}
}
