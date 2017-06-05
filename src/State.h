#pragma once

//Add new states here
enum StateType
{
	WAIT,
	FOLLOW,
	DEFAULT
};

//To be used in a state machine
class State
{
public:
	/*
	When we create our NPC's add functions like belo
	virtual void Execute(WaitingNPC* waitingNPC) = 0;
	*/

	virtual void Execute() = 0;
};

class State_Wait : public State
{
public:
	/*
	Execute will look something like,
	void Execute(WaitingNpc* waitingNPC)
	{
		if(waitingNpc->shouldWait())
		{
			waitingNPC->wait();
		}

		else
		{
		    waitingNPC->ChangeState(new State_Follow);
		}
	}
	*/

	void Execute() {};


};

class State_Follow : public State
{
public:
	void Execute() {};
	
};