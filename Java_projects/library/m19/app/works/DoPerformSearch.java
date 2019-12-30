package m19.app.works;

import java.util.List;

import m19.core.LibraryManager;

import m19.core.Work;

import m19.app.works.Message;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;


/**
 * 4.3.3. Perform search according to miscellaneous criteria.
 */
public class DoPerformSearch extends Command<LibraryManager> {

  private Input<String> _searchTerm;

  /**
   * @param m
   */
  public DoPerformSearch(LibraryManager m) {
    super(Label.PERFORM_SEARCH, m);
    _searchTerm = _form.addStringInput(Message.requestSearchTerm());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() {
    _form.parse();
    List<Work> _matches = _receiver.doPerformSearch(_searchTerm.value());
    for(Work w : _matches)
      _display.addLine(w.toString());
    
    _display.display();
  }
  
}
