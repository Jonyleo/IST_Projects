package m19.app.users;

import java.util.List;

import m19.core.LibraryManager;

import m19.core.exception.InvalidUserIdException;

import m19.app.users.Message;

import m19.app.exception.NoSuchUserException;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;

/**
 * 4.2.3. Show notifications of a specific user.
 */
public class DoShowUserNotifications extends Command<LibraryManager> {

  private Input<Integer> _userId;

  /**
   * @param receiver
   */
  public DoShowUserNotifications(LibraryManager receiver) {
    super(Label.SHOW_USER_NOTIFICATIONS, receiver);
    _userId = _form.addIntegerInput(Message.requestUserId());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    _form.parse();
    try {
      List<String> notifications = _receiver.doShowUserNotifications(_userId.value());
      for(String s : notifications)
        _display.addLine(s);
      _display.display();
    } catch( InvalidUserIdException iuie) {
      throw new NoSuchUserException(_userId.value());
    }
  }

}
