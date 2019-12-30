package m19.app.users;

import m19.core.LibraryManager;

import m19.core.exception.InvalidUserIdException;

import m19.app.users.Message;

import m19.app.exception.NoSuchUserException;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;
import pt.tecnico.po.ui.DialogException;

/**
 * 4.2.2. Show specific user.
 */
public class DoShowUser extends Command<LibraryManager> {

  private Input<Integer> _userToShow;

  /**
   * @param receiver
   */
  public DoShowUser(LibraryManager receiver) {
    super(Label.SHOW_USER, receiver);
    _userToShow = _form.addIntegerInput(Message.requestUserId());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    _form.parse();
    try {
      _display.popup(_receiver.doShowUser(_userToShow.value()));
    } catch( InvalidUserIdException iuie) {
      throw new NoSuchUserException(_userToShow.value());
    }
  }
}
