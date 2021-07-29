# Coding Guidelines

# Introduction

This document is intended to establish a common coding guideline that should be followed for extensions and pull requests. It largely follows recommendations given by Robert C. Martin in his book [Clean Code: A Handbook of Agile Software Craftsmanship](https://www.oreilly.com/library/view/clean-code-a/9780136083238/) but may deviate in some places. This document provides general coding guidelines (not just formatting guidelines) that indicate how this code is developed.

The main motivation of vocell::cfd is to provide a set of tools that extend the standard c++ library and functionalities to perform general-purpose CFD simulations at high-performance without trying to impose a certain programming believe-system on the developer and/or user. For example, vocell::cfd makes use of static code analysing tools that check for c++ best practices and coding styles. We do not invent our own best practices but rather follow established community guidelines here. This not only helps to promote high code quality but also helps to keep this document short and digestable. Furthermore, seasoned c++ programmers will find their way around the code easily as it will follow guidelines that may be familiar from other projects. Input scripts are based on the [JSON](https://www.json.org/json-en.html) file format which is widely used elsewhere and should be familiar to many programmers and potentially users.

The following content will be covered in this document:

* [1. Naming Convention](#Naming-Convention)
* [2. Comments](#Comments)
* [3. Code Documentation](#Code-Documentation)
* [4. Formatting](#Formatting)
* [5. Classes and Structs](#Classes-and-Structs)
* [6. Static Code Checking](#Static-Code-Checking)
* [7. Testing](#Testing)
* [8. Software Versions and Releases](#Software-Versions-and-Releases)

# Naming convention

Variables, functions and methods should follow the camelCase naming convention. Single worded variables should remain lowercase.

```{.cpp}
// variables
double result = 0;
int customIndex = 0;

// function / methods
void logInformation(std::string logMessage) { ... }
double get() { return ... }
```

Custom types (such as classes, structs, templates, typedefs and Enums) should follow the PascalCase naming convention. The entries of enums, however, should follow a SCREAMING_SNAKE_CASE naming convention. This allows distinguishing Enums for namespaces in a given context. Private and protected variable names should be prefixed by an underscore. This may be discouraged by some c++ guidelines and the underscore should be placed after the variable name. In this way, however, private variables can not be immediately distinguished from public variables and may look overall clumsy. Other languages actively encourage and in-fact expect private variables to be prefixed with underscores (e.g. Dart). Therefore, private and protected variables should be prefixed and not postfixed with the underscore.

```{.cpp}
// enums
enum FileType{
  ASCII = 0,
  BINARY
};

// classes / structs
template<typename FloatType, class LogType>
class PostProcessing {
public:
  using MessageType = typename LogType::MessageType;
  ...
private:
  const std::string _filename;
};
```

Variables should have _intention-revealing_ names and should in general not be abbreviated. For example, instead of using `std::string msg`, it is preferred to use `std::string message`. As a general rule of thumb, if the variable name is not clear and need to be documented with a comment, it should be renamed so that the comment can be removed and the variable name serves as documentation itself.

```{.cpp}
// variables
double result; // BAD: Result of what?
double AccumulatedResidualL2ErrorNorm; // GOOD: Variable's name reveals intention;

// function / methods
void calc() { ... } // BAD: Function name abbreviated, what is calculated here?
void calculateDragForceCoefficient() { ... } // GOOD: No abbreviations, clear intention
```

The intention here is that variable and function names will most of the time be seen out of context (for example, a method of a class will be called within another class where no information about the called function is available (unless documentation or the source code is consulted).
Variables may be abbreviated as a reference and reused within a function / method as long as it is only used in the current scope.

```{.cpp}
calculateDragForceCoefficient(const Field &pressureField) {
  const auto &p = pressureField;
  ...
  auto cd = integrate(p, ...);
}
```

As a general rule of thumb, you should not use single worded variable of function / method names unless they are bound to the current scope. In the example above, using the single-worded function name ```void calculate() { ... }``` is dangerous as it does not reveal its intention. The version ```void calculateDragForce() { ... }``` is to be preferred here. Variables should, in general, follow the same reasoning but may resort to single-worded names if their meaning is unambiguous. Consider the following definition:

```{.cpp}
const Field<Scalar> _gradient;
```

This definition provides additional information about the gradient type (it is a scalar field). If this variable is only used within the current class, this name may be acceptable (as its definition and thus type information is sufficiently close to where the variable is used). If it is a protected member and the definition is obscured through several layers of inheritance, then it may be beneficial to provide some more information about the variable within its name. For example, we could then replace the above name with the following:

```{.cpp}
const Field<Scalar> _scalarGradientField;
```

Depending on the context, this may be even further modified to include the field name of which the gradient is being computed. For example, if this was done for the pressure, we could also write

```{.cpp}
const Field<scalar> _pressureGradient;
```

which, in this context, would probably be the most appropriate choice. In General, this needs to be decided on a case by case basis and common sense applies here.

Namespaces should follow the same naming convention as methods / functions and variables. The following serves as an example:

```{.cpp}
namespace vocell::linearAlgebra { }
```

File names should also follow the camelCase naming contention, for example, use `myClass.hpp`, `myClass.cpp` and `myClass.tpp` as file names.

The above introduced naming schemes for all variables is summarised in the following table:

| Entity                      | Naming Convention    |
|-----------------------------|----------------------|
| Class / Struct Enum / name  | PascalCase           |
| Enum entries                | SCREAMING_SNAKE_CASE |
| Header include guards       | SCREAMING_SNAKE_CASE |
| Typedefs                    | PascalCase           |
| namespaces                  | camelCase            |
| class methods and functions | camelCase            |
| variable names              | camelCase            |
| file names                  | camelCase            |

# Comments
Similar to variable names, the source code should generally be _intention-revealing_, i.e. comments should be kept to a minimum and time should be spent on finding good variable names and extract code snippets into separate functions / methods or lambda expressions if it promotes better code understanding. Comments should never be used to state obvious facts, for example, comments of the following nature are not allowed:

```{.cpp}
int main() {
  // set up logging class object
  LoggingClass logObject();
  
   ...

  // get a full error report from the logging class object
  auto errorReport = lobObject.getFullErrorReport();
}
```

The code is self-evident without the comments, so they should be removed. This promotes compact codes and further reduces the burden of maintaining comments. Consider the following example:

```{.cpp}
int main() {
  ...
  // generate VTK file output
  postProcessor.generateOutput();
  ...
}
```

Now we rely on the comment to know what output we generate (here a VTK file). Say the implementation of ```generateOutput()``` changes and we are writing the output now as a CGNS file. In this case, the above comment would be broken (in fact, it is more dangerous as it is actively misleading). This highlights two poor programming choices:

* relying on comments to document non-intention revealing code
* poor naming convention

The above could be easily fixed by providing a better API. For example, the original version may be refactored to read

```{.cpp}
int main() {
  ...
  postProcessor.generateVTKOutput();
  ...
}
```

No comments are necessary here and the function name reveals intention. SHould we want to change the internal implementation, an argument should be made here to keep the existing ```generateVTKOutput()``` method and create a second ```generateCGNSOutput()``` method, both of which can coexist. If we ever wanted to change the file writing, then we could simply swap the function name to

```{.cpp}
int main() {
  ...
  postProcessor.generateCGNSOutput();
  ...
}
```

Both versions are clear and do not require a comment to explain what the function is doing. Thus we do not risk having misleading or incorrect comments after parts of the code is refactored and therefore do not need to maintain comments. The only time comments are acceptable is when a certain design decision was made which can not be expressed in code or something is not immediately obvious by looking at just the code itself. These could be, for example, certain algorithms written in a non-standard way to improve performance or complicated algorithms for which only sparse or no documentation exists elsewhere for consultation. In any case, comments in source code should be avoided as much as possible and comments should be primarily used outside functions / methods, ideally only outside classes where the intention and purpose of a class and its methods (API) is documentated.

# Code Documentation

The current project uses doxygen to document code in lack of viable alternatives. We try to restrict code documentation here as much as possible to classes alone, i.e. each class must have a brief one-line description that describes the purpose of the class in less than 25 words without using the words "if", "and", "or" or "but" (see [Classes and Structs](#Classes-and-Structs) for the rationale behind this). Member functions should not not be documented. Consider the following method which sets the current time (in hours, minutes and seconds):

```{.cpp}
/// Set the current time using the current hour, minute and second
/** @param hour The current hour to set
  * @param minute The current minute to set
  * @param second The current second to set
  */
void setCurrentTime(int hour, int minute, int second) {
  _hour = hour;
  _minute = minute;
  _second = second;
}
```

The additional documentation does not add any additional information (nor is any additional information required here as the naming is sufficiently intention-revealing and clear). Thus, the following code, without any documentation is entirely equivalent in terms of clearness but reduces unnecessary documentation (which could be considered white noise here):

```{.cpp}
void setCurrentTime(int hour, int minute, int second) {
  _hour = hour;
  _minute = minute;
  _second = second;
}
```

This is an example that has been chosen on purpose to be rather trivial. There will be times, however, where some functions require additional information to help someone working with the class to get their way around. In most cases, we are interested in using the class somewhere else, thus, what we really should be discussing is the public API / interface of the class and how a user of this class may reasonably expect to use it. Consider the following class

```{.cpp}
class ReadUnstructuredMesh {
...
public:
  readCoordinates( ... ) { ... }
  readBoundaryConditions( ... ) { ... }
  readConnectivityTable( ... ) { ... } 
...
};
```

It may be necessary that the user needs to call all three functions in order to set up the computational mesh. These functions may be different for a structured mesh and thus we should document its usage. We should note here, that in both cases a better design decision could be to create an abstract interface from which both structured and unstructured mesh readers derive from which then provide, for example, a public interface method called ```readMesh()``` or similar. It may, however, be necessary to provide some intermediate API for a separate class, say, ```template<class MeshReader>class ComputationalMesh { ... };```, which would need fine granined control access to the structured and unstructured mesh class and based on the template argument make a decision which methods to use. In this case, we should provide sufficient information in the ```ReadUnstructuredMesh``` class so that someone implementing ```ComputationalMesh``` knows which methods to call. Therefore, each class should provide at the beginning the following boiler plate code which is to be maintained by the implementer of the class:

```{.cpp}
/** \class Name_of_class
  * \brief A one line description of the class
  * \ingroup unique ID associating current class with a group of classes
  *
  * Detailed description
  *
  * \code
  * // show how to use this class in code
  * \ endcode (remove space, only used here to not end documentation block prematurely)
  */
```

In the case of the mesh reader interface class discussed above, the following example code documentation could be provided:

```{.cpp}
/** \class MeshReaderInterface 
  * \brief Expose unstructured mesh information of a mesh read from disk
  * \ingroup MeshReaderGroup
  * 
  * This class reads a generic unstructured file from disk and provides
  * public methods to get information about the coordinates, the
  * boundary conditions and the connectivity table of the vertices.
  *
  * You may typically want to call the following methods after
  * you have created a ReadUnstructuredMesh object:
  * 
  * \code
  * ReadUnstructuredMesh unstructuredMesh();
  * 
  * const auto cooridnates = unstructuredMesh.readCoordinates();
  * const auto boundaryConditions = unstructuredMesh.readBoundaryConditions();
  * const auto connectivityTable = unstructuredMesh.readConnectivityTable();
  * \ endcode (remove space, only used here to not end documentation block prematurely)
  * 
  * coordinates can be index as a 3D array, for example as coordinate[i][j][k]
  * The boundary conditions have a 2D index, the first being the boundary ID (and its size
  * corresponds to the number of boundary conditions available) while the second index is used
  * to address the boundary faces in turn on that boundary.
  * The connectivity table also uses a 2D indexing where the first index goes through all cells
  * in the mesh, while the second goes through all vertices in each cell. 
  */
class ReadUnstructuredMesh {
...
public:
  readCoordinates( ... ) { ... }
  readBoundaryConditions( ... ) { ... }
  readConnectivityTable( ... ) { ... } 
...
};
```

More documentation may be appropriate here but it should just give a flavour of what sort of information to provide. You may want to check the source code for some classes to get a better understanding.

# Formatting

vocell::cfd follows the [Google style guide](https://google.github.io/styleguide/cppguide.html) in terms of code 
formatting with minor modifications. We do not, however, follow the rest of the coding guidelines. For example, Google does not allow to throw exceptions which form an integral part of vocell::cfd. To see a list of all formatting rules, you may want to run clang-format with the following command ```clang-format -style=google --dump-config``` and consult the [clang format](https://clang.llvm.org/docs/ClangFormatStyleOptions.html) documentation for each variables' meaning.

vocell::cfd deviates slightly from the Google style guide as mentioned above. These deviations are listed below. 
Each clang-format variable is listed along with the new value used here (along with the value of Google in 
parenthesis which is overridden). To make things clear, each deviation is exemplified through a code snippet.

* AccessModifierOffset: -2 (-1)
```{.cpp}
// google style guide
class Base {
 public:
  Base();
};

// vocell::cfd style guide
class Base {
public:
  Base();
};
```
* ColumnLimit: 120 (80)
```{.cpp}
// Changes the number of characters allowed per line to 120.
```

* AlignAfterOpenBracket: DontAlign (Align)
```{.cpp}
// google style guide
void add(int a, // assume line break here, for example, max line length reached
         int b);

// vocell::cfd style guide
void add(int a, // assume line break here, for example, max line length reached
  int b);
```

* ContinuationIndentWidth: 2 (4)
```{.cpp}
// related to AlignAfterOpenBracket above
// will determine how much next line argument will be indented
```

* AllowShortLambdasOnASingleLine: Empty (All)
```{.cpp}
// google style guide
executeLambda([](){});
executeLambda([](){ return 42; });

// vocell::cfd style guide
executeLambda([](){}); // allow empty lambdas on single line
executeLambda([](){    // break non-empty lambdas into several lines 
  return 42;
});
```

# Classes and Structs

Classes should be used whenever program logic is implemented and structs only as data containers. From this it follows that structs can never have any methods, nor constructors or destructors. Consider the following example, showing how classes and structs have different responsibilities.

```{.cpp}
// use a struct here to store a coordinate point
template<typename FloatType>
struct Coordinate {
  FloatType x;
  FloatType y;
  FloatType z;
};

// use the coordinate struct within the computational mesh class
template<typename FloatType>
class ComputationalMesh {
public:
  void setCoordinate(FloatType x, FloatType y, FloatType z) {
    _coordinate.x = x;
    _coordinate.y = y;
    _coordinate.z = z;
  }
private:
  Coordinate<FloatType> _coordinate;
};
```

Classes should have a single responsibility and never do more than that. We should be able to describe the purpose of each class in about 25 words without using the words "if", "and", "or" or "but" (this needs to be provided as code documentation, see the relevant [Code Documentation](#Code-Documentation) section). If we can't do that, the current class likely has too many responsibilities and should be refactored into smaller units.

Classes should always be split into definition (header) and implementation (source) files. Header files should have a file extension of *.hpp while source files should use *.cpp. No implementation should be done in the header files, only definitions are allowed here. This may be complicated if templates are used; in this case, a third file with the extension *.tpp should be used and included _outside_ the class and at the end of the file. This will effectively include the definition in the header file itself but not compile it. Picking up the previous computational mesh example, this would now be split in the following way.

```{.cpp}
// ComputationalMesh.hpp file
template<typename FloatType>
class ComputationalMesh {
public:
  void setCoordinate(FloatType x, FloatType y, FloatType z);
private:
  Coordinate<FloatType> _coordinate;
};

#include "ComputationalMesh.tpp"

// ComputationalMesh.tpp
template<typename FloatType>
void ComputationalMesh<FloatType>::setCoordinate(FloatType x, FloatType y, FloatType z) {
  _coordinate.x = x;
  _coordinate.y = y;
  _coordinate.z = z;
}
```

Note that variable names are provided both in the definition and implementation file inside the function signature. This promotes readability in the header file and is therefore a necessary requirement. Furthermore, forcing the implementation outside the class definition (header file), readibility is further improved.

When templates are used, c++ allows both ```typename``` and ```class``` as an identifier. Here we use ```typename``` if the underlying type is essentially mimicking a build-in variable type (like float, double or int) and ```class``` for custom program logic types. For example, should we want to write a complex number class, that would essentially mimick a built-in type (with some extensions) and therefore ```typename``` should be used rather than ```class``` here. On the other hand, should we want to generate different types of computational mesh classes, then we should use ```class``` rather than ```typename``` should we want to pass that type as a template parameter to another class. Template types should also always be postfixed with the word _Type_ to make sure that template types are easily recognised throughout the codebase. In code this would mean:

```{.cpp}
// typename example
template<typename ComplexNumberType>
class SomeClass { ... };

// class example
template<class ComputationalMeshType>
class SomeOtherClass { ... };
``` 

Classes may be derived to leverage polymorphism in which case a parent class can either be a _Base_ or _Interface_ class. An _interface_ class must have at least one pure virtual function whereas a _Base_ class may consist of a mix of both normal and virtual methods. Furthermore, an _Interface_ should largely delegate implementation responsibilities to derived classes while _Base_ classes may implement some core logic themselves and only be extended by other derived classes.

To keep the public interface clean and readable, public interface methods should only call private implementation methods where implementation details are hidden and not implement any logic themselves. This restriction may be ignored if the implementation itself is rather trivial (for example, getter and setter functions). A reasonable rule of thumb is that if a public method requires more than 5 lines of code to implement a certain logic, it should delegate the implementation details to a private method and make use of it instead. Common programming paradigms take precedence over this rue, however. For example, if a short implementation can be provided for a certain public interface method, which would be required in other methods as well (public and/or private), it should still be implemented as a private method, i.e. DRY (don't repeat yourself) takes precedence here.

# Static Code Checking

In order to comply with common c++ best practices, static code checking is performed using clang-tidy and cppcheck. Each merge into the master branch must produce a clang-tidy and cppcheck output that does not show any error. It is intended to comply with as many best practices as possible, i.e. clang-tidy should be run with ```-checks=*```. However, should there ever be a conflicting standpoint, the [c++ core guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines) should take priority. It may be necessary to turn off certain guidelines if they are incompatible with a design decision and those exceptions will be listed here.

# Testing

vocell::cfd is developed following a strict test-driven development (TDD) approach. We follow mainly the guidelines provided by [Khorikov](https://www.manning.com/books/unit-testing), where the core logic of the software should be covered by unit, integration and system tests. This has been adapted [here](https://github.com/tomrobin-teschner/softwareTesting) for scientific software development which is the basic guideline that is followed here.
The testing framework employed here is [googletest](https://github.com/google/googletest). Unit, integration and system tests are strictly separated into their own testing environment (indicated by their sub-folders within the ```tests/``` directory) and which test needs to be provided depends on the class and feature developed. As a rule of thumb, we do not aim for 100% code coverage but rather on testing core business logic of the code. For example, private methods in classes should not be tested, as they hide implementation logic. Here, rather the public method calling that private method should be tested which in turn will implicitly test both methods. This is illustrated in the following:

```{.cpp}
class ReadMesh {
public:
  void readMeshFromFile( ... ) {
     // calling parseStringAndSplitIntoArguments() somewhere
  }

private:
  std::vector<std::string> parseStringAndSplitIntoArguments( ... ) { ... }
};
```

In the above example, we would only test ```readMeshFromFile( ... )``` and not ```parseStringAndSplitIntoArguments( ... )``` as that would be testing for implementation details. We should only ever test the public interface / API and ensure its correctness, from which it follows that implementation details are working correctly for the cases tested. It may be beneficial here to do parametric testing to cover as many boundary cases as possible, i.e. if ```parseStringAndSplitIntoArguments( ... )``` contains several ```if/else``` branches, we may want to run enough tests so that each branch is covered.

While a more complete reference of the testing guidelines adopted here can be found [here](https://github.com/tomrobin-teschner/softwareTesting), the following serves as general rules that should be followed:

* Do not test private methods in classes, only the public interface / API of a class
* Test for outcomes and not for implementations. For example, if an output file is generated, do not parse the file and check that it is identical to another file which is the expteced output but rather test that all impiortant pieces are there (and only check for the right order if absolute necessary, i.e. if the file requires it). This makes the test less brittle against possible extensions so that tests do not fail if additional components are added to the output file.
* Only test core business logic. For example, an overloaded stream operator may be omitted if its sole purpose is to simplify debugging or logging of information. If the logging of information, however, is deemed to be crucial, a test should be provided.
* Consider to write a unit test for each public method that represents core business logic. If the class uses several public methods to create a certain state (for example, a mesh reading class may read the coordinates, connectivity and boundary information to construct a useful computational mesh), an integration test should be provided as well. A system test should be provided to test the system itself, for example, an incompressible solver may be considered a system which tests the mesh reading, time integration, spatial reconstruction and file writing. It is not always clear how to draw the boundaries, a more detailed discussion is given in this [manuscript](https://github.com/tomrobin-teschner/softwareTesting/blob/master/A_practical_guide_towards_agile_test-driven_development_for_scientific_software_projects.pdf)

# Software Versions and Releases

[Semantic versioning](https://semver.org/) is adopted here which follows a MAJOR.MINOR.PATCH naming convention (i.e. 3.4.1, for example). As outlined on the website above, to update either MAJOR, MINOR or PATCH, the following rules are adopted:

* PATCH: The PATCH number is increased whenever a bugfix or patch is introduced. This increment happens on the current version, irrespective of when the original feature was introduced. Say, for example, the development is currently at version 1.18.0 and a bug is discovered which was introduced in version 1.2.0. In this case, the bug is applied to the current version 1.18.0 and incremented, i.e. the new version will be 1.18.1, i.e. we do not checkout version 1.2.0 and apply the patch here (version 1.2.1) and then cherrypick it ontop of the current development branch master.

* MINOR: Whenever a new feature is introduced (typically a new class is added), this should result in the MINOR version being incremented and the PATCH version being reset to 0. If a feature is added to an existing class that only affects the implementation details, then the PATCH version should be incremented. If the change brings about a new feature being available, then the MINOR version should be incremented.

* MAJOR: According to [semantic versioning](https://semver.org/), MAJOR should be incremented whenever backwards-incompatible changes are introduced to the public API. Since this project is concerned with building an executable and not a library, backwards-incompatible changes occur only for input files and possibly custom scripting through the Python interface. Thus, if changes are introduced which break these functionalities (rather than the public API of classes, which will only be used by other classes but not by the user itself), MAJOR will be incremented, but changes to the public API will be tolerated as MINOR or PATCH releases, if they have no bearing on how the software is used (which is at least in the spirit of semantic versioning). The only exception here is the change from version 0.x.x to version 1.x.x which signals a stable API and user input structure. Features introduced in version 0.x.x are allowed to completely break the workflow and file input structure on the user-side.

The current version will always be displayed in the top-level CMakeLists.txt file and the documentation includes a changelog in which each version and its changes are documented. Release candidates will be tagged and made available on GitHub along with an overview of changes introduced since the last release. Each MAJOR number also receives a release name.