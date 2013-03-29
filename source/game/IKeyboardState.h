#ifndef GB_IKEYBOARDSTATE_H
#define GB_IKEYBOARDSTATE_H

#include <map>
#include <string>
#include <vector>

typedef std::vector<int> Keys;
typedef std::map<std::string, int> KeyNameMap;

class IKeyboardState
{
	public:
		virtual ~IKeyboardState() {}
	
	public:
		virtual void setKeyDown(int index) = 0;
		virtual void setKeyDown(std::string keyName) = 0;
		virtual void setKeyUp(int index) = 0;
		virtual void setKeyUp(std::string keyName) = 0;
		virtual bool getKeyState(int index) = 0;
		virtual bool getKeyState(std::string keyName) = 0;
		virtual void setKeyNameMapping(KeyNameMap mapping) = 0;
};

#endif
