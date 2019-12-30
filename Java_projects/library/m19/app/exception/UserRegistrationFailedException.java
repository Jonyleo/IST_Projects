package m19.app.exception;

import pt.tecnico.po.ui.DialogException;

/**
 * Class encoding user registration failure.
 */
public class UserRegistrationFailedException extends DialogException {
  
  /** Serial number for serialization. */
  static final long serialVersionUID = 201901091828L;

  /** Bad user's name. */
  private String _name;
  
  /** Bad user's email. */
  private String _email;

  /**
   * @param name
   * @param email
   */
  public UserRegistrationFailedException(String name, String email) {
    _name = name;
    _email = email;
  }


  /** @see pt.tecnico.po.ui.DialogException#getMessage() */
  @Override
  public String getMessage() {
    return Message.userRegistrationFailed(_name, _email);
  }
  
}
