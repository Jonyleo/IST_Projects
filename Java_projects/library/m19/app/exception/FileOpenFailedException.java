package m19.app.exception;

import pt.tecnico.po.ui.DialogException;

/**
 * Class encoding problems processing input files.
 */
public class FileOpenFailedException extends DialogException {

  /** Serial number for serialization. */
  static final long serialVersionUID = 201901091828L;

  /**
   * Bad file name.
   */
  private String _name;

  /**
   * @param name
   */
  public FileOpenFailedException(String name) {
    _name = name;
  }

  /** @see pt.tecnico.po.ui.DialogException#getMessage() */
  @Override
  public String getMessage() {
    return Message.fileNotFound(_name);
  }

}
