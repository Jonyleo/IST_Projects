package m19.core;

import java.util.List;

import java.io.IOException;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.ObjectOutputStream;
import java.io.FileInputStream;
import java.io.ObjectInputStream;

import m19.core.exception.MissingFileAssociationException;
import m19.core.exception.BadEntrySpecificationException;
import m19.core.exception.BadEntryUserException;
import m19.core.exception.ImportFileException;
import m19.core.exception.InvalidWorkIdException;
import m19.core.exception.InvalidUserIdException;
import m19.core.exception.InvalidRequestException;
import m19.core.exception.CoreRuleFailedException;
import m19.core.exception.NotSuspendedException;

/**
 * The façade class.
 */
public class LibraryManager {

  private Library _library = new Library(); 
  private String _fileAssociation;

  public void doAdvanceDate(Integer n) {
    _library.advanceDate(n.intValue());
  }

  public int doDisplayDate() {
    return _library.getDate();
  }

  public int doRegisterUser(String name, String email) throws BadEntryUserException {
    return _library.registerUser(name, email);
  }

  public String doShowUser(Integer id) throws InvalidUserIdException {
    return _library.getUser(id).toString();
  }

  public List<User> doShowUsers() {
    return _library.getUsers();
  }

  public String doDisplayWork(Integer id) throws InvalidWorkIdException {
    return _library.getWork(id).toString();
  }

  public List<Work> doDisplayWorks() {
    return _library.getWorks();
  }

  public int requestWork(Integer workId, Integer userId) throws CoreRuleFailedException, InvalidWorkIdException, InvalidUserIdException {
    return _library.requestWork(workId, userId);
  }

  public int returnWork(Integer workId, Integer userId) throws InvalidWorkIdException, InvalidUserIdException, InvalidRequestException {
    return _library.returnWork(workId, userId);
  }

  public void doPayFine(Integer userId) throws InvalidUserIdException, NotSuspendedException {
    _library.payFine(userId);
  }
  
  public void addReturnNotification(Integer workId, Integer userId) throws InvalidWorkIdException, InvalidUserIdException {
    _library.addReturnNotification(workId, userId);
  } 

  public void addRequestNotification(Integer workId, Integer userId) throws InvalidWorkIdException, InvalidUserIdException {
    _library.addRequestNotification(workId, userId);
  } 

  public List<String> doShowUserNotifications(Integer userId) throws InvalidUserIdException {
    return _library.getUserNotifications(userId);
  }

  public List<Work> doPerformSearch(String searchTerm) {
    return _library.getSearchResults(searchTerm.toLowerCase());
  }

  public List<String> doIncreasePrice(String searchTerm) {
    return _library.increasePrice(searchTerm);
  }

  public String doShowValuedUser() {
    return _library.getValuedUserDesc();
  }

  /**
   * Serialize the persistent state of this application.
   * 
   * @throws MissingFileAssociationException if the name of the file to store the persistent
   *         state has not been set yet.
   * @throws IOException if some error happen during the serialization of the persistent state

   */
  public void save() throws MissingFileAssociationException, IOException {
    if( _fileAssociation == null )
      throw new MissingFileAssociationException();

    saveAs(_fileAssociation);
  }

  /**
   * Serialize the persistent state of this application into the specified file.
   * 
   * @param filename the name of the target file
   *
   * @throws MissingFileAssociationException if the name of the file to store the persistent
   *         is not a valid one.
   * @throws IOException if some error happen during the serialization of the persistent state
   */
  public void saveAs(String filename) throws MissingFileAssociationException, IOException {
    
    try(FileOutputStream fileOut = new FileOutputStream(filename); ObjectOutputStream outStream = new ObjectOutputStream(fileOut)) {
      outStream.writeObject(_library);
      _fileAssociation = filename;
    } catch ( FileNotFoundException fnfe ) {
      throw new MissingFileAssociationException();
    } 
  }

  /**
   * Recover the previously serialized persitent state of this application.
   * 
   * @param filename the name of the file containing the perssitente state to recover
   *
   * @throws IOException if there is a reading error while processing the file
   * @throws FileNotFoundException if the file does not exist
   * @throws ClassNotFoundException 
   */
  public void load(String filename) throws FileNotFoundException, IOException, ClassNotFoundException {
    try(FileInputStream fileIn = new FileInputStream(filename); ObjectInputStream inStream = new ObjectInputStream(fileIn)) {
      _library = (Library) inStream.readObject();
      _fileAssociation = filename;
    }
  }

  /**
   * Set the state of this application from a textual representation stored into a file.
   * 
   * @param datafile the filename of the file with the textual represntation of the state of this application.
   * @throws ImportFileException if it happens some error during the parsing of the textual representation.
   */
  public void importFile(String datafile) throws ImportFileException {
    try {
      _library.importFile(datafile);
    } catch (IOException | BadEntrySpecificationException | BadEntryUserException e) {
      throw new ImportFileException(e);
    }
  }
}
