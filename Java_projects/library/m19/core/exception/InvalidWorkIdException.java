package m19.core.exception;

/**
 * Exception for invalid work id
 */
public class InvalidWorkIdException extends Exception {
  private int _invalidId;

  public InvalidWorkIdException(int id) {
    _invalidId = id;
  }

  public int getInvalidId() {
    return _invalidId;
  }
}