#ifndef action_h
#define action_h




class Action_node
{


public:
	Action_node(): next(NULL) {}
	~Action_node();
	enum {key_node, text_node} type;

	union{
		short key;			//vk
		struct {
			char * utf8;	//utf8 text for paste，utf-8要用WideCharToMultiByte中的LPSTR存
			short length;
		} text;
	}data;

	Action_node * next;

};

class Action
{

public:
	static void SetHHelper(HWND hWC3_helper) { Action::hWC3_helper = hWC3_helper; }
	static void SetHWar3(HWND hWar3) { Action::hWar3 = hWar3; }
	Action(): action_steps(NULL) {}
	~Action();
	void Parse(WCHAR* target);
	void Do(bool bKeyUp);

private:
	static HWND hWar3;
	static HWND hWC3_helper;
	Action_node *action_steps;


};






#endif