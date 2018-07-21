
#pragma once

#include "log/sevarity.hpp"

// - There is a way to get the default logger which could be a global or singleton
// - able to write to standard out && error
// - able to add a file to output to
// - able to add a ostream to write to
// - have a concept of a sink that the output will be written to;
// - set if the logger is async or not
// - able to set the format of the output for one or all syncs
// - syncs subscribe to a level. either just that level or a minimim range.
//

