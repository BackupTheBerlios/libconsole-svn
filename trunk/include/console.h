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
#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <string>
#include <vector>

namespace Console
{
	/*! Object that should be modified by the console should inherit from this class. */
	class Listener
	{
	public:
		/*!
		 * \param name Unique ID used by the console manager. */
		inline explicit Listener(const std::string &name) : mName(name) {}
		inline virtual ~Listener() {}

		/*! This inherited function should be used to set up the commands the console
		    manager should be able recognize. */
		virtual void initialize() = 0;

		inline std::string getName() { return mName; }

	protected:
		std::string mName;
	};

	/*! The console manager uses references to the modifyable objects.
	 * \param name Name identifier
	 * \param function Function identification name
	 * \param object Pointer to the object to be used */
	class Ref
	{
	public:
		std::string name;
		std::string function;
		void*       object;
	};

	/*! Function pointer for an int call.
	 * \param Pointer to the function to be called */
	class Ref1i : public Ref
	{
	public:
		void (*functionPtr)(void* object, int param);
	};

	/*! Function pointer for a float call.
	* \param Pointer to the function to be called */
	class Ref1f : public Ref
	{
	public:
		void (*functionPtr)(void* object, float param);
	};

	/*! Console manager
	 * \author Joachim Klahr
	 * \version 0.1.0
	 * \date 2006 */
	class Manager
	{
	public:
		static Manager* singleton();

		/*! Removes the singleton instance. */
		void clear();

		/*! Performs the console action.
		 * \param command Form: [objectname] [functionname] [value] */
		void executeCommand(const std::string &command);

		/*! Adds references to objects and function
		 * \param name Object name.
		 * \param function Identification name of function.
		 * \param object Pointer to the object, usually "this".
		 * \param functionPtr Pointer to the function to be executed. */
		void addRef1i(const std::string &name, const std::string &function,
			          void* object, void (*functionPtr)(void* object, int param));
		void addRef1f(const std::string &name, const std::string &function,
			          void* object, void (*functionPtr)(void* object, float param));

		/*! Removes the specified reference.
		 * \param name Object name.
		 * \param function Function name. */
		void removeRef1i(const std::string &name, const std::string &function);
		void removeRef1f(const std::string &name, const std::string &function);

	private:
		Manager();
		~Manager();

		/*! Splits the command line up in words.
		 * \param command Command line. Ex. "mycar setspeed 10". */
		std::vector<std::string> parse(const std::string &command);

		static Manager* mSingleton;

		std::vector<Ref1i> mRef1is;
		std::vector<Ref1f> mRef1fs;
	};
}

#endif
