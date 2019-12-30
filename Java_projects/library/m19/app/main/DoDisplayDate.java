package m19.app.main;

import m19.core.LibraryManager;

import m19.app.main.Message;

import pt.tecnico.po.ui.Command;
 
/**
 * 4.1.2. Display the current date.
 */
public class DoDisplayDate extends Command<LibraryManager> {

  /**
   * @param receiver
   */
  public DoDisplayDate(LibraryManager receiver) {
    super(Label.DISPLAY_DATE, receiver);
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() {
    _display.popup(Message.currentDate(_receiver.doDisplayDate()));
  }
  
}
