package m19.core.exception;

public class CoreRuleFailedException extends Exception {
	private int _id;

	public CoreRuleFailedException(int id) {
		_id = id;
	}

	public int getId() {
		return _id;
	}
}