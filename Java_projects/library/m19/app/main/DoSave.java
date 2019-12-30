package m19.app.main;

import java.io.IOException;

import m19.core.LibraryManager;
import m19.core.exception.MissingFileAssociationException;

import m19.app.main.Message;
import m19.app.exception.FileOpenFailedException;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;
import pt.tecnico.po.ui.DialogException;

/**
 * 4.1.1. Save to file under current name (if unnamed, query for name).
 */
public class DoSave extends Command<LibraryManager> {
  
  private Input<String> _file;

  /**
   * @param receiver
   */
  public DoSave(LibraryManager receiver) {
    super(Label.SAVE, receiver);
    _file = _form.addStringInput(Message.newSaveAs());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException{
    try {
      _receiver.save();

    } catch ( MissingFileAssociationException mfae ) {
      _form.parse();

      try {
        _receiver.saveAs(_file.value());

      } catch( MissingFileAssociationException | IOException e ) {
        throw new FileOpenFailedException(_file.value());
      }
      
    } catch ( IOException ioe ) {
      throw new FileOpenFailedException(_file.value());  
    }
  }
}
