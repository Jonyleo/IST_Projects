package m19.app.main;

/** Messages for menu interactions. */
@SuppressWarnings("nls")
public interface Message {

  /**
   * @return string with prompt for filename to open.
   */
  static String openFile() {
    return "Ficheiro a abrir: ";
  }

  /**
   * @return string confirming that user wants to save.
   */
  static String saveBeforeExit() {
    return "Guardar antes de fechar? ";
  }

  /**
   * @return string asking for a filename.
   */
  static String saveAs() {
    return "Guardar ficheiro como: ";
  }

  /**
   * @return string with a warning and a question.
   */
  static String newSaveAs() {
    return "Ficheiro sem nome. " + saveAs();
  }

  /**
   * <code>actual</code> gera a mensagem que indica a data actual ao sistema. O
   * valor da data é medido em dias e é um número inteiro.
   * 
   * @param dataActual
   *          é a data actual do sistema.
   * @return <code>String</code> com a mensagem.
   */
  static String currentDate(int dataActual) {
    return "Data actual: " + dataActual;
  }

  /**
   * <code>pedidoDeDias</code> gera a mensagem que indica que é necessário
   * fornecer um número de dias ao sistema, para fazer avançar a data. O valor
   * da data é medido em dias e é um número inteiro.
   * 
   * @return <code>String</code> com a mensagem
   */
  static String requestDaysToAdvance() {
    return "Introduza número de dias a avançar: ";
  }

}
