package m19.core;

import java.util.Map;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;

import java.io.Serializable;

import m19.core.exception.BadEntryUserException;

public class User implements Serializable, Observer{
	private final Integer _id;
	private final String _name;
	private final String _email;
	private boolean _isSuspended;
	private AbstractStatus _status;
	private final Map<Integer, Request> _requests = new HashMap<Integer, Request>();
	private int _countDelayed;
	private int _countOnTime;
	private int _fine;
	private final List<String> _notifications = new ArrayList<String>();


	User(String name, String email, Integer id) throws BadEntryUserException {
		if(name.length() == 0 || email.length() == 0 || name.equals(email))
			throw new BadEntryUserException();
		_name = name;
		_email = email;
		_id = id;
		_status = NormalStatus.get();
	}

	public String getName() {
		return _name;
	}
	int getId() {
		return _id;
	}

	boolean isSuspended() {
		return _isSuspended;
	}

	void payFine() {
		_fine = 0;
	}

	int getFine() {
		return _fine; 
	}

	void updateSuspended(int currDay) {
		if(_fine != 0) {
			_isSuspended = true;
			return;
		}
		_isSuspended = false;
		for(Request r : _requests.values()) {
			if(r.isLate(currDay)) {
				_isSuspended = true;
				break;
			}
		}
	}

	AbstractStatus getStatus() {
		return _status;
	}

	/*Request related methods*/
	int getNumberOfRequests() {
		return _requests.size();
	}

	boolean isRequested(Work work) {
		return _requests.containsKey(work.getId());
	}

	void addRequest(Request request) {
		_requests.put(request.getId(), request);
	}

	Request getRequest(Work work) {
		return _requests.get(work.getId());
	}

	void returnRequest(Request request, int currDay) {
		_requests.remove(request.getId());
		
		if(request.isLate(currDay)) {
			_countDelayed++;
			_countOnTime = 0;
		}
		else {
			_countOnTime++;
			if(_countOnTime >= 3 || _countDelayed < 3)
				_countDelayed = 0;
		}

		_fine += request.getFine(currDay);

		updateStatus();
	}


	void updateStatus() {
		if(_countDelayed >= 3)
			_status = FaltosoStatus.get();
		else if(_countOnTime >= 5)
			_status = CumpridorStatus.get();
		else
			_status = NormalStatus.get();
	}

	public void notify(String notification) {
		_notifications.add(notification);
	}

	void clearNotifications() {
		_notifications.clear();
	}

	List<String> getNotifications() {
		return new ArrayList<String>(_notifications);
	}

	int getValue() {
		int value = 0;
		for(Request r : _requests.values())
			if(r.getWork().getPrice() > value)
				value = r.getWork().getPrice();

		return value;
	}


	/*Built-in methods (Overriden)*/
	@Override
	public String toString() {
		return "" + _id + " - " + _name + " - " + _email + " - " + getStatus() + " - " + (isSuspended() ? "SUSPENSO - " + "EUR " + _fine : "ACTIVO");
	}
}