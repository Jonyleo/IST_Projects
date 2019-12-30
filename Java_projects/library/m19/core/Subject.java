package m19.core;

import java.util.List;
import java.util.ArrayList;

import java.io.Serializable;

abstract class Subject implements Serializable {
	protected List<Observer> _observers = new ArrayList<Observer>();

	abstract void update();
	void addObserver(Observer obs) {
		_observers.add(obs);
	}
	void removeObserver(Observer obs) {
		_observers.remove(obs);
	}
}