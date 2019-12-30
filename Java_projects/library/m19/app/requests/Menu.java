package m19.app.requests;

import m19.core.LibraryManager;
import pt.tecnico.po.ui.Command;

/** 4.4. Requests menu. */
public class Menu extends pt.tecnico.po.ui.Menu {

  /**
   * @param receiver
   */
  public Menu(LibraryManager receiver) {
    super(Label.TITLE, new Command<?>[] { //
      new DoRequestWork(receiver), //
      new DoReturnWork(receiver), //
    });
  }

}
