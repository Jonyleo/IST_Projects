package m19.app.works;

import m19.core.LibraryManager;
import pt.tecnico.po.ui.Command;

/** 4.3. Works menu. */
public class Menu extends pt.tecnico.po.ui.Menu {

  /**
   * @param receiver
   */
  public Menu(LibraryManager receiver) {
    super(Label.TITLE, new Command<?>[] { //
      new DoDisplayWork(receiver), //
      new DoDisplayWorks(receiver), //
      new DoPerformSearch(receiver), //
      new DoIncreasePrice(receiver), //
    });
  }

}
