package m19.app.users;

/** Messages for menu interactions. */
@SuppressWarnings("nls")
public interface Message {

  /**
   * @return message
   */
  static String requestUserId() {
    return "Introduza o número de utente: ";
  }

  /**
   * @return message
   */
  static String requestUserName() {
    return "Introduza o nome do utente: ";
  }

  /**
   * @return message
   */
  static String requestUserEMail() {
    return "Introduza o endereço de correio do utente: ";
  }

  /**
   * @param idUser
   * @return message
   */
  static String userRegistrationSuccessful(int idUser) {
    return "Novo utente criado com o número " + idUser + ".";
  }

}
