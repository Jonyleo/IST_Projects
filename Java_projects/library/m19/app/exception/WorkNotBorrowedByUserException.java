package m19.app.exception;

import pt.tecnico.po.ui.DialogException;

/**
 * Class encoding return failure.
 */
public class WorkNotBorrowedByUserException extends DialogException {
  
  /** Serial number for serialization. */
  static final long serialVersionUID = 200510291601L;

  /** Bad user id. */
  private int _idUser;

  /** Bad work id. */
  private int _idWork;

  /**
   * @param idWork
   * @param idUser
   */
  public WorkNotBorrowedByUserException(int idWork, int idUser) {
    _idWork = idWork;
    _idUser = idUser;
  }

  /** @see pt.tecnico.po.ui.DialogException#getMessage() */
  @Override
  public String getMessage() {
    return Message.workNotBorrowedByUser(_idWork, _idUser);
  }
  
}
