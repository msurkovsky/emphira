
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

using namespace clang::tooling;
using namespace llvm

static cl:OptionCategory MyToolCategory("my-tool options");

static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

static cl::extrahelp MoreHelp("\nMore help text ...");

int main(int argc, const cahr **argv) {
  
  CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);

  ClangTool Tool(OptionParser.getCompilations(),
                 OptionParser.getSourcePathList());
  return Tool.run(new FrontendActionFactory<clang::SyntaxOnlyAction>().get());
}
