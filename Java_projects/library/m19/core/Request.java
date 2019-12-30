package m19.core;

import java.io.Serializable;

class Request implements Serializable {
	private int _deadline;
	private Work _work;
	private User _user;

	Request(int deadline, Work work, User user) {
		_deadline = deadline;
		_work = work;
		_user = user;
	}

	Work getWork() {
		return _work;
	}

	User getUser() {
		return _user;
	}

	int getId() {
		return _work.getId();
	}

	void requestSelf() {
		_user.addRequest(this);
    	_work.requestWork();
	}

	void returnSelf(int time) {
		_user.returnRequest(this, time);
		_work.returnWork();
	}

	boolean isLate(int currDay) {
		return currDay > _deadline;
	}

	int getFine(int currDay) {
		if(!isLate(currDay))
			return 0;
		return (currDay - _deadline) * 5;
	}
}