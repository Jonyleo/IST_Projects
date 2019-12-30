package m19.app.exception;

import pt.tecnico.po.ui.DialogException;

/**
 * Class encoding the failure of borrowing requests.
 */
public class RuleFailedException extends DialogException {

  /** Serial number for serialization. */
  static final long serialVersionUID = 200510291601L;

  /** User id. */
  int _idUser;

  /** Work id. */
  int _idWork;

  /** Index of failed rule. */
  int _ruleIndex = -1;

  /**
   * @param idUser
   * @param idWork
   * @param ruleIndex
   */
  public RuleFailedException(int idUser, int idWork, int ruleIndex) {
    _idUser = idUser;
    _idWork = idWork;
    _ruleIndex = ruleIndex;
  }

  /**
   * @return index of failed rule
   */
  public int getRuleIndex() {
    return _ruleIndex;
  }

  /**
   * @return work id
   */
  public int getWork() {
    return _idWork;
  }

  /**
   * @return user id
   */
  public int getUser() {
    return _idUser;
  }

  /** @see pt.tecnico.po.ui.DialogException#getMessage() */
  @Override
  public String getMessage() {
    return Message.ruleFailed(_idUser, _idWork, _ruleIndex);
  }

}
