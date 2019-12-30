package m19.app.requests;

import m19.core.LibraryManager;

import m19.core.exception.InvalidUserIdException;
import m19.core.exception.InvalidWorkIdException;
import m19.core.exception.InvalidRequestException;
import m19.core.exception.NotSuspendedException;

import m19.app.exception.NoSuchUserException;
import m19.app.exception.NoSuchWorkException;
import m19.app.exception.WorkNotBorrowedByUserException;
import m19.app.exception.UserIsActiveException;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
import pt.tecnico.po.ui.Form;

/**
 * 4.4.2. Return a work.
 */
public class DoReturnWork extends Command<LibraryManager> {

  private Input<Integer> _workId;
  private Input<Integer> _userId;
  private Input<Boolean> _fineQuery;
  private Form _formQuery;

  /**
   * @param receiver
   */
  public DoReturnWork(LibraryManager receiver) {
    super(Label.RETURN_WORK, receiver);
    _formQuery = new Form("Boas");

    _userId = _form.addIntegerInput(Message.requestUserId());
    _workId = _form.addIntegerInput(Message.requestWorkId());
    _fineQuery = _formQuery.addBooleanInput(Message.requestFinePaymentChoice());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    _form.parse();
    try {
      int fine = _receiver.returnWork(_workId.value(), _userId.value());
      
      if(fine == 0) 
        return;
      
      _display.popup(Message.showFine(_userId.value(), fine));

      _formQuery.parse();
      if(_fineQuery.value()) 
        _receiver.doPayFine(_userId.value());
      

    } catch ( InvalidWorkIdException iwie ) {
      throw new NoSuchWorkException(_workId.value());
    } catch( InvalidUserIdException iuie ) {
      throw new NoSuchUserException(_userId.value());
    } catch ( InvalidRequestException ire ) {
      throw new WorkNotBorrowedByUserException(_workId.value(), _userId.value());
    } catch ( NotSuspendedException nfe ) {
      throw new UserIsActiveException(_userId.value());
    } 
  }

}
