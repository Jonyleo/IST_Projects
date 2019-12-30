package m19.app.users;

import m19.core.LibraryManager;

import m19.core.exception.NotSuspendedException;
import m19.core.exception.InvalidUserIdException;

import m19.app.users.Message;

import m19.app.exception.UserIsActiveException;
import m19.app.exception.NoSuchUserException;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;

/**
 * 4.2.5. Settle a fine.
 */
public class DoPayFine extends Command<LibraryManager> {

  private Input<Integer> _userId;

  /**
   * @param receiver
   */
  public DoPayFine(LibraryManager receiver) {
    super(Label.PAY_FINE, receiver);
    _userId = _form.addIntegerInput(Message.requestUserId());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    _form.parse();
    try {
      _receiver.doPayFine(_userId.value());
    } catch ( NotSuspendedException nfe ) {
      throw new UserIsActiveException(_userId.value());
    } catch( InvalidUserIdException iuie ) {
      throw new NoSuchUserException(_userId.value());
    }
  }
}
