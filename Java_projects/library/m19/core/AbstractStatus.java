package m19.core;

import java.io.Serializable;

abstract class AbstractStatus implements Serializable {
	private int _maxWorks;
	private String _statusString;

	protected AbstractStatus(int maxWorks, String ststring) {
		_maxWorks = maxWorks;
		_statusString = ststring;
	}

	int getMaxWorks() {
		return _maxWorks;
	}

	abstract boolean canRequestWithPrice(int price);
	abstract int getDeadline(int nCopies);

	@Override
	public String toString() {
		return _statusString;
	}
}