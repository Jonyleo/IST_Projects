package m19.app.requests;

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
  static String requestWorkId() {
    return "Introduza o número da obra: ";
  }

  /**
   * @param idUtente
   * @param euros
   * @return message
   */
  static String showFine(int idUtente, int euros) {
    return "O utente " + idUtente + " deve pagar uma multa de EUR " + euros + ".";
  }

  /**
   * @return message with yes/no question.
   */
  static String requestFinePaymentChoice() {
    return "O utente deseja pagar multa (s/n)? ";
  }

  /**
   * @return message
   */
  static String requestReturnNotificationPreference() {
    return "Deseja ser avisado quando algum exemplar for devolvido (s/n)? ";
  }

  /**
   * @param idWork
   * @param day
   * @return message
   */
  static String workReturnDay(int idWork, int day) {
    return "A obra " + idWork + " deve ser devolvida no dia " + day + ".";
  }

}
