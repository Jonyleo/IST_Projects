package m19.core;

import java.io.Serializable;

abstract class AbstractNotification extends Subject {
	private String _msg;
	private Work _work;

	protected AbstractNotification(String msg, Work work) {
		_msg = msg;
		_work = work;
	}

	String getMsg() {
		return _msg + _work;
	}

	@Override
	void update() {
		for(Observer obs : _observers)
			obs.notify(getMsg());
	}
}