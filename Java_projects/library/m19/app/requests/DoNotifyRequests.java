package m19.app.requests;

import m19.core.LibraryManager;

import m19.core.exception.InvalidUserIdException;
import m19.core.exception.InvalidWorkIdException;
import m19.core.exception.InvalidRequestException;
import m19.core.exception.NotSuspendedException;

import m19.app.exception.NoSuchUserException;
import m19.app.exception.NoSuchWorkException;
import m19.app.exception.WorkNotBorrowedByUserException;
import m19.app.exception.UserIsActiveException;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
import pt.tecnico.po.ui.Form;

/**
 * 4.4.2. Return a work.
 */
public class DoNotifyRequests extends Command<LibraryManager> {

  private Input<Integer> _workId;
  private Input<Integer> _userId;

  /**
   * @param receiver
   */
  public DoNotifyRequests(LibraryManager receiver) {
    super("Adiconar notifcacao de entrega", receiver);

    _userId = _form.addIntegerInput(Message.requestUserId());
    _workId = _form.addIntegerInput(Message.requestWorkId());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    _form.parse();
    try {
      _receiver.addRequestNotification(_workId.value(), _userId.value());

      _display.popup("User will be notified");      

    } catch ( InvalidWorkIdException iwie ) {
      throw new NoSuchWorkException(_workId.value());
    } catch( InvalidUserIdException iuie ) {
      throw new NoSuchUserException(_userId.value());
    }
  }

}
