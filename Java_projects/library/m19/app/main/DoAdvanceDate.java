package m19.app.main;

import m19.core.LibraryManager;

import m19.app.main.Message;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;

/**
 * 4.1.3. Advance the current date.
 */
public class DoAdvanceDate extends Command<LibraryManager> {

  private Input<Integer> _dateToAdvance;

  /**
   * @param receiver
   */
  public DoAdvanceDate(LibraryManager receiver) {
    super(Label.ADVANCE_DATE, receiver);
    _dateToAdvance = _form.addIntegerInput(Message.requestDaysToAdvance());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() {
    _form.parse();
    if(_dateToAdvance.value() > 0)
      _receiver.doAdvanceDate(_dateToAdvance.value());
  }
}
