package m19.app.exception;

/** Messages for menu interactions. */
@SuppressWarnings("nls")
public interface Message {

  /**
   * The work corresponding to the id does not exist.
   * 
   * @param idWork
   * @return message
   */
  static String noSuchWork(int idWork) {
    return "A obra " + idWork + " não existe.";
  }

  /**
   * The user corresponding to the id does not exist.
   * 
   * @param idUser
   * @return message
   */
  static String noSuchUser(int idUser) {
    return "O utente " + idUser + " não existe.";
  }

  /**
   * The user corresponding to the id is not suspended.
   * 
   * @param idUser
   * @return message
   */
  static String userNotSuspended(int idUser) {
    return "O utente " + idUser + " não se encontra suspenso.";
  }

  /**
   * generates message with the rule violation
   * 
   * @param idUser
   * @param idWork
   * @param ixRule
   * @return messgae
   */
  static String ruleFailed(int idUser, int idWork, int ixRule) {
    return "O utente " + idUser + " não pode requisitar a obra " + idWork + ". Violação da regra " + ixRule + ".";
  }

  /**
   * @param idWork
   * @param idUser
   * @return message
   */
  static String workNotBorrowedByUser(int idWork, int idUser) {
    return "A obra " + idWork + " não foi requisitada pelo utente " + idUser + ".";
  }

  /**
   * @param name
   * @param email
   * @return message
   */
  static String userRegistrationFailed(String name, String email) {
    return "User registration failed: name '" + name + "', email '" + email + "'.";
  }

  /**
   * @return string with "file not found" message.
   */
  static String fileNotFound() {
    return "O ficheiro não existe.";
  }

  /**
   * @param filename
   * @return string with "file not found" message (more elaborate).
   */
  static String fileNotFound(String filename) {
    return "O ficheiro '" + filename + "' não existe.";
  }

}
