package m19.app.requests;

import m19.core.LibraryManager;

import m19.core.exception.CoreRuleFailedException;
import m19.core.exception.InvalidUserIdException;
import m19.core.exception.InvalidWorkIdException;

import m19.app.exception.RuleFailedException;
import m19.app.exception.NoSuchUserException;
import m19.app.exception.NoSuchWorkException;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
import pt.tecnico.po.ui.Form;

/**
 * 4.4.1. Request work.
 */
public class DoRequestWork extends Command<LibraryManager> {

  private Input<Integer> _workId;
  private Input<Integer> _userId;
  private Input<Boolean> _notificationQuery;
  private Form _formQuery;

  /**
   * @param receiver
   */
  public DoRequestWork(LibraryManager receiver) {
    super(Label.REQUEST_WORK, receiver);
    _formQuery = new Form("Boas");


    _userId = _form.addIntegerInput(Message.requestUserId());
    _workId = _form.addIntegerInput(Message.requestWorkId());
    _notificationQuery = _formQuery.addBooleanInput(Message.requestReturnNotificationPreference());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    _form.parse();
    try {
      int deadline = _receiver.requestWork(_workId.value(), _userId.value());
      _display.popup(Message.workReturnDay(_workId.value(), deadline));
      
    } catch ( CoreRuleFailedException crfe ) {
      if( crfe.getId() != 3 )
        throw new RuleFailedException(_userId.value(), _workId.value(), crfe.getId());

      _formQuery.parse();

      try {
        if(_notificationQuery.value()) 
          _receiver.addReturnNotification(_workId.value(), _userId.value());
        
      } catch ( InvalidWorkIdException iwie ) {
        throw new NoSuchWorkException(_workId.value());
      } catch( InvalidUserIdException iuie ) {
        throw new NoSuchUserException(_userId.value());
      } 
    } catch ( InvalidWorkIdException iwie ) {
      throw new NoSuchWorkException(_workId.value());
    } catch( InvalidUserIdException iuie ) {
      throw new NoSuchUserException(_userId.value());
    } 
  }
}
