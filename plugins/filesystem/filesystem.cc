// MIT License
//
// Copyright (c) 2020 Paweł Adamski
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <cstdlib>
#include <iostream>

#include <spaghetti/element.h>
#include <spaghetti/logger.h>
#include <spaghetti/registry.h>

#ifdef BUILD_PLUGIN_GUI
#include <spaghettiui/node.h>
#include "dir_list/dir_list_node.h"
#include "dir_select/dir_select_node.h"
#include "file_select/file_select_node.h"
#else
#include <spaghetti/dummynode.h>
#endif

#include "dir_list/dir_list.h"
#include "dir_select/dir_select.h"
#include "file_select/file_select.h"

using namespace spaghetti;

extern "C" SPAGHETTI_API void register_plugin(spaghetti::Registry &a_registry)
{
  spaghetti::log::init_from_plugin();

#ifdef BUILD_PLUGIN_GUI
  a_registry.registerElement<elements::DirSelect, nodes::DirSelect>("Dir Select", ":/unknown.png");
  a_registry.registerElement<elements::DirList, nodes::DirList>("Dir List", ":/unknown.png");
  a_registry.registerElement<elements::FileSelect, nodes::FileSelect>("File Select", ":/unknown.png");
#else
  a_registry.registerElement<elements::DirSelect>("Dir Select", ":/unknown.png");
  a_registry.registerElement<elements::DirList>("Dir List", ":/unknown.png");
  a_registry.registerElement<elements::FileSelect>("File Select", ":/unknown.png");
#endif
}
