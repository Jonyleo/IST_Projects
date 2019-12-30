package m19.core;

import java.io.Serializable;

import java.util.List;
import java.util.ArrayList;

import m19.core.exception.BadEntrySpecificationException;

public abstract class Work implements Serializable {
	
	private final Integer _id;
	private final String _title;
	private int _price;
	private final int _totalCopies;
	private int _requestedCopies;
	private Category _category;
	private ReturnNotification _returnSubject = new ReturnNotification(this);
	private RequestNotification _requestSubject = new RequestNotification(this);


	Work(Integer id, String title, int price, int total, String cat) {
		_id = id;
		_title = title;
		_price = price;
		_totalCopies = total;
		_requestedCopies = 0;
		_category = Category.valueOf(cat);
	}

	public Integer getId() {
		return _id;
	}

	int getAvailableCopies() {
		return _totalCopies - _requestedCopies;
	}

	int getTotalCopies() {
		return _totalCopies;
	}

	Category getCategory() {
		return _category;
	}

	int getPrice() {
		return _price;
	}

	void requestWork() {
		_requestedCopies++;
		
		_requestSubject.update();
	}

	void returnWork() {
		_requestedCopies--;

		_returnSubject.update();
	}

	void addRequestObserver(User user) {
		_requestSubject.addObserver(user);
	}

	void addReturnObserver(User user) {
		_returnSubject.addObserver(user);
	}

	boolean matches(String searchTerm) {
		return _title.toLowerCase().contains(searchTerm) || matchCreator(searchTerm);
	}

	String getTitle() {
		return _title;
	}

	int increasePrice() {
		_price += 10;
		return _price;
	}

	boolean matchesTitle(String searchTerm) {
		return _title.contains(searchTerm);
	}

	protected abstract boolean matchCreator(String searchTerm);
	protected abstract String getSpecialEnding();
	protected abstract String getType();

	@Override
	public String toString() {
		return "" + _id + " - " + getAvailableCopies() + " de " + _totalCopies + " - " + getType() + " - " +
		_title + " - " + _price + " - " + _category.getString() + " - " + getSpecialEnding();
	}
}