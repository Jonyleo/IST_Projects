package m19.app.works;

import m19.core.LibraryManager;

import m19.core.exception.InvalidWorkIdException;

import m19.app.works.Message;
import m19.app.exception.NoSuchWorkException;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;
import pt.tecnico.po.ui.DialogException;

/**
 * 4.3.1. Display work.
 */
public class DoDisplayWork extends Command<LibraryManager> {

  private Input<Integer> _workToShow;

  /**
   * @param receiver
   */
  public DoDisplayWork(LibraryManager receiver) {
    super(Label.SHOW_WORK, receiver);
    _workToShow = _form.addIntegerInput(Message.requestWorkId());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    _form.parse();
    try {
      _display.popup(_receiver.doDisplayWork(_workToShow.value()));
    } catch( InvalidWorkIdException iwie ) {
      throw new NoSuchWorkException(_workToShow.value());
    }
  }
}
