package m19.core.exception;

public class NotSuspendedException extends Exception {
	private int _userId;

	public NotSuspendedException(int userId) {
		_userId = userId;
	}

	public int getUserId() {
		return _userId;
	}
}