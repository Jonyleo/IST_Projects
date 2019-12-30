package m19.core.exception;

public class InvalidRequestException extends Exception {
	private int _workId;
	private int _userId;

	public InvalidRequestException(int workId, int userId) {
		_workId = workId;
		_userId = userId;
	}

	public int getInvalidWorkId() {
		return _workId;
	}

	public int getInvalidUserId() {
		return _userId;
	}
}