package m19.core.exception;

/**
 * Exception for invalid user id
 */
public class InvalidUserIdException extends Exception {
  private int _invalidId;

  public InvalidUserIdException(int id) {
    _invalidId = id;
  }

  public int getInvalidId() {
    return _invalidId;
  }
}
