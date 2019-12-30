package m19.app.users;

import m19.core.LibraryManager;
import pt.tecnico.po.ui.Command;

/** 4.2. Users menu. */
public class Menu extends pt.tecnico.po.ui.Menu {

  /**
   * @param receiver
   */
  public Menu(LibraryManager receiver) {
    super(Label.TITLE, new Command<?>[] { //
      new DoRegisterUser(receiver), //
      new DoShowUser(receiver), //
      new DoShowUsers(receiver), //
      new DoShowUserNotifications(receiver), //
      new DoPayFine(receiver), //
      new DoShowValuedUser(receiver), //
    });
  }

}
