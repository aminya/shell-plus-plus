// Copyright 2016 Alex Silva Torres
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "runner.h"

#include <cstdlib>

#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <readline/readline.h>

namespace seti {

Runner::Runner() {}

void Runner::Exec(std::string name) {
  try {
    interpreter_.Exec(name);
  } catch (RunTimeError& e) {
    std::cout << "Error: " << e.pos().line << ": " << e.pos().col
              << ": " << e.what() << "\n\n";

    for (auto& msg: e.messages()) {
      std::cout << "Error: " << msg.line() << ": " << msg.pos()
                << ": " << msg.msg() << "\n";
    }
  }
}

void Runner::ExecInterative() {
  interpreter_.ExecInterative([](bool concat){
    char *input;
    std::string str_source;

    if (concat) {
      input = readline("| ");
    } else {
      input = readline("> ");
    }

    if (input == nullptr) {
      exit(0);
    }

    str_source = input;
    free(input);
    return str_source;
  });
}

}