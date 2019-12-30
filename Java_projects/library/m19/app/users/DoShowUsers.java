package m19.app.users;

import m19.core.LibraryManager;

import m19.core.User;

import java.util.List;
import java.util.Comparator;

import pt.tecnico.po.ui.Command;

/**
 * 4.2.4. Show all users.
 */
public class DoShowUsers extends Command<LibraryManager> {

  /**
   * @param receiver
   */
  public DoShowUsers(LibraryManager receiver) {
    super(Label.SHOW_USERS, receiver);
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() {
    List<User> userArray = _receiver.doShowUsers();

    userArray.sort(Comparator.comparing(u -> u.getName().toLowerCase()));

    for(User u : userArray)
      _display.addLine(u.toString());
   
    _display.display();    
  }
}
