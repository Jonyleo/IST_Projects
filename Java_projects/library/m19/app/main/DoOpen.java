package m19.app.main;

import java.io.FileNotFoundException;
import java.io.IOException;

import m19.core.LibraryManager;

import m19.app.exception.FileOpenFailedException;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;
import pt.tecnico.po.ui.DialogException;

/**
 * 4.1.1. Open existing document.
 */
public class DoOpen extends Command<LibraryManager> {

  private Input<String> _filename;

  /**
   * @param receiver
   */
  public DoOpen(LibraryManager receiver) {
    super(Label.OPEN, receiver);
    _filename = _form.addStringInput(Message.openFile());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    try {
      _form.parse();
      _receiver.load(_filename.value());

    } catch ( ClassNotFoundException | IOException e) {
      throw new FileOpenFailedException(_filename.value());
    } 
    
  }

}
