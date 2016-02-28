# Installation #
  * Copy the dll into the bin directory, located in the Tecplot installation directory.
  * Edit your tecplot.add file ,located in the Tecplot installation directory, and append this line:<br>$LoadAddon "createtetzone"</li></ul>

<h1>Usage #

  * Launch Tecplot, and load your data.
  * Go to Tools>Create Tetrahedral Zone.
  * Select the source zones to use. If multiple zones are selected, the points from all zones will be used to create the tetrahedral zone.
  * Click compute, and it will either succeed or give an error message.
  * Close the dialog. Go to Data>Data Set Info, and verify that a new zone called "Tetrahedral zone" was added to the data set.