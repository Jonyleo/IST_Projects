package m19.core;

import java.util.Map;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;
import java.util.Comparator;

import java.io.Serializable;
import java.io.IOException;

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
 * Class that represents the library as a whole.
 */
public class Library implements Serializable {

  /** Serial number for serialization. */
  private static final long serialVersionUID = 201901101348L;
  
  private int _time;

  private Integer _totalUsers = Integer.valueOf(0);
  private Integer _totalWorks = Integer.valueOf(0);

  private Map<Integer, User> _users = new HashMap<Integer, User>();
  private Map<Integer, Work> _works = new HashMap<Integer, Work>();

  private List<AbstractRule> _rules = new ArrayList<AbstractRule>();

  Library() {
    _rules.add(new NoTwoWorksRule(1));
    _rules.add(new NotSuspendedRule(2));
    _rules.add(new NoAvailableCopiesRule(3));
    _rules.add(new MaxWorksRequestedRule(4));
    _rules.add(new NoReferenceWorksRule(5));
    _rules.add(new PriceLimitReachedRule(6));
  }

  /**
   * Read the text input file at the beginning of the program and populates the
   * instances of the various possible types (books, DVDs, users).
   * 
   * @param filename
   *          name of the file to load
   * @throws BadEntrySpecificationException
   * @throws IOException
   */
  void importFile(String filename) throws BadEntrySpecificationException, BadEntryUserException, IOException {
    Parser parser = new Parser(this);
    parser.parse(filename);
  }


  /**
   * Advances the date
   *
   * @param d number of days to advance
   */
  void advanceDate(int d) {
    _time += d;
    for(User u : _users.values()) 
      u.updateSuspended(_time);
  }

  /**
   * Gets the date
   *
   * @return current date
   */
  int getDate() {
    return _time;
  }

  /**
   * Registers a user in the library
   *
   * @throws BadEntryUserException when either the name or email is an empty string ""
   *
   * @param name string containing the user's name
   * @param email string containing the user's email
   *
   * @return id of the new user
   */
  Integer registerUser(String name, String email) throws BadEntryUserException {
    Integer uid = _totalUsers;
    User u = new User(name, email, uid);
    _users.put(uid, u);
    _totalUsers++;
    return uid;
  }

  /**
   * Gets the user with the specific id
   *
   * @throws InvalidUserIdException when the id provided is invalid
   *
   * @param id user id
   *
   * @return the user with the given id
  */
  User getUser (Integer id) throws InvalidUserIdException {
    User u = _users.get(id);
  	if(u == null) 
      throw new InvalidUserIdException(id);
    return u;
  }

  /**
   * Gets a list of users
   *
   * @return User list containing all the registered users
  */
  List<User> getUsers() {
    return new ArrayList<User>(_users.values());
  }

  /**
   * Gets  the work with the specific id
   * 
   * @throws InvalidWorkIdException when the id provided is invalid
   *
   * @param id work id
   *
   * @return the work with the given id
  */
  Work getWork (Integer id) throws InvalidWorkIdException {
  	Work w = _works.get(id);

  	if(w == null) 
      throw new InvalidWorkIdException(id);
    return w;
  }

  /**
   * Gets a list of works
   *
   * @return User list containing all the created works
  */
  List<Work> getWorks() {
  	return new ArrayList<Work>(_works.values());
  }

  /**
   * Creates a book and adds it to the library
   * 
   * @param title title of the book
   * @param price price of the book
   * @param total total ammount of copies
   * @param cat category of the book
   * @param wri writer of the book
   * @param isbn isbn of the book
  */
  void addBook(String title, int price, int total, String cat, String wri, String isbn) {
    _works.put(_totalWorks, new Book(_totalWorks, title, price, total, cat, wri, isbn));
    _totalWorks++;
  }

  /**
   * Creates a dvd and adds it to the library
   * 
   * @param title title of the dvd
   * @param price price of the dvd
   * @param total total ammount of copies
   * @param cat category of the dvd
   * @param dir director of the dvd
   * @param igac igac of the dvd
  */
  void addDvd(String title, int price, int total, String cat, String dir, String igac) {
    _works.put(_totalWorks, new Dvd(_totalWorks, title, price, total, cat, dir, igac));
    _totalWorks++;
  }


  /**
   * Requests a work for a user
   *
   * @param workId id of the work to request
   * @param userId id of the user to make the request
   *
   * @throws CoreRuleFailedException if a rule fails
   * @throws InvalidWorkIdException if the given workid does not match a valid work
   * @throws InvalidUserIdException if the given userid does not match a valid user
   *
  */
  int requestWork(Integer workId, Integer userId) throws CoreRuleFailedException, InvalidWorkIdException, InvalidUserIdException {
    Work w = getWork(workId);
    User u = getUser(userId);

    for(AbstractRule r : _rules)
      r.testRule(w, u);

    int deadline = u.getStatus().getDeadline(w.getTotalCopies()) + _time;

    Request r = new Request(deadline, w, u);
    r.requestSelf();

    return deadline;
  }

  /**
   * Returns a work for a user
   *
   * @param workId id of the work to return
   * @param userId id of the user to make the return
   *
   * @throws InvalidWorkIdException if the given workid does not match a valid work
   * @throws InvalidUserIdException if the given userid does not match a valid user
   * @throws InvalidRequestException if the user hasn't requested the given work
   *
  */
  int returnWork(Integer workId, Integer userId) throws InvalidWorkIdException, InvalidUserIdException, InvalidRequestException {
    Work w = getWork(workId);
    User u = getUser(userId);

    if(!u.isRequested(w))
      throw new InvalidRequestException(workId, userId);

    Request r = u.getRequest(w);

    r.returnSelf(_time);

    return u.getFine();
  }


  /**
   * Pays a user's fine, updating the suspended state
   *
   * @param userId id of the user to pay fine
   *
   * @throws InvalidUserIdException if the given userid does not match a valid user
   * @throws NotSuspendedException if the user does not have an associated fine
   *
  */
  void payFine(Integer userId) throws InvalidUserIdException, NotSuspendedException {
    User u = getUser(userId);
    if(!u.isSuspended())
      throw new NotSuspendedException(userId);

    u.payFine();
    u.updateSuspended(_time);
  }

  /**
   * Makes a user notifiable after a return of a work
   *
   * @param workId work to observe
   * @param userId user that observes
   *
   * @throws InvalidWorkIdException if the given workid does not match a valid work
   * @throws InvalidUserIdException if the given userid does not match a valid user
   *
  */
  void addReturnNotification(Integer workId, Integer userId) throws InvalidWorkIdException, InvalidUserIdException {
    Work w = getWork(workId);
    User u = getUser(userId);

    w.addReturnObserver(u);
  }

 /**
   * Makes a user notifiable after a request of a work
   *
   * @param workId work to observe
   * @param userId user that observes
   *
   * @throws InvalidWorkIdException if the given workid does not match a valid work
   * @throws InvalidUserIdException if the given userid does not match a valid user
   *
  */
  void addRequestNotification(Integer workId, Integer userId) throws InvalidWorkIdException, InvalidUserIdException {
    Work w = getWork(workId);
    User u = getUser(userId);

    w.addRequestObserver(u);
  }

  /**
   * Returns a list of notification descriptions for a given user
   *
   * @param userId user to show
   *
   * @throws InvalidUserIdException if the given userid does not match a valid user
  */
  List<String> getUserNotifications(Integer userId) throws InvalidUserIdException {
    User u = getUser(userId);
    List<String> notifications = u.getNotifications();
    u.clearNotifications();
    return notifications;
  }

  /**
   * Returns a list with all the works that match a specific searchTerm
   *
   * @param searchTerm the pattern to compare each work to
  */
  List<Work> getSearchResults(String searchTerm) {
    List<Work> _matches = new ArrayList<Work>();
    for(Work w : _works.values())
      if(w.matches(searchTerm))
        _matches.add(w);
    return _matches;
  }


  List<String> increasePrice(String searchTerm) {
    List<String> matches = new ArrayList<String>();
    for(Work w : _works.values())
      if(w.matchesTitle(searchTerm))
        matches.add("" + w.getTitle() + " - " + w.increasePrice() + " EUR");
    return matches;
  }

  String getValuedUserDesc() {
    if(_users.size() == 0)
      return null;

    List<User> usersOrdered = new ArrayList<User>(_users.values());
    usersOrdered.sort(Comparator.comparing(u -> u.getId()));
    User userValued = null;

    for(User u : usersOrdered) {
      if(userValued == null)
        userValued = u;

      else if(u.getValue() >= userValued.getValue())
        userValued = u;
    }

    return userValued.toString();
  }

}

