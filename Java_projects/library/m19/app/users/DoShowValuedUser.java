package m19.app.users;

import m19.core.LibraryManager;

import m19.core.User;

import java.util.List;
import java.util.Comparator;

import pt.tecnico.po.ui.Command;

/**
 * 4.2.4. Show all users.
 */
public class DoShowValuedUser extends Command<LibraryManager> {

  /**
   * @param receiver
   */
  public DoShowValuedUser(LibraryManager receiver) {
    super("Utente mais Valioso", receiver);
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() {
    String userdesc = _receiver.doShowValuedUser();
    if(userdesc == null)
      return;
    _display.popup(userdesc);
  }
}
