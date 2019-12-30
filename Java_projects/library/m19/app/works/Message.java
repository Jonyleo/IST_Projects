package m19.app.works;

/** Messages for menu interactions. */
@SuppressWarnings("nls")
public interface Message {

  /**
   * @return message
   */
  static String requestWorkId() {
    return "Introduza o número da obra: ";
  }

  /**
   * @param idWork
   * @return message
   */
  static String semExemplares(int idWork) {
    return "A obra " + idWork + " não pode ser requisitada: não há exemplares disponíveis.";
  }

  /**
   * @return message
   */
  static String requestSearchTerm() {
    return "Introduza o termo de pesquisa: ";
  }

}
