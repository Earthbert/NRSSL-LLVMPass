#!/bin/sh

# Remove all existing alternatives
update-alternatives --remove-all llvm
update-alternatives --remove-all clang

# exit on first error
set -eu
version=${1:-19}
priority=${2:-30}

# sudo apt update
# sudo apt install --assume-yes \
    # "clang-$version" \
    # "clang-$version-doc" \
    # "clang-format-$version" \
    # "clang-tools-$version" \
    # "clangd-$version" \
    # "libc++-$version-dev" \
    # "libc++abi-$version-dev" \
    # "libclang-$version-dev" \
    # "libclang-common-$version-dev" \
    # "libclang1-$version" \
    # "libfuzzer-$version-dev" \
    # "libllvm$version" \
    # "libllvm-$version-ocaml-dev" \
    # "lld-$version" \
    # "lldb-$version" \
    # "llvm-$version" \
    # "llvm-$version-dev" \
    # "llvm-$version-doc" \
    # "llvm-$version-examples" \
    # "llvm-$version-runtime"

# llvm
update-alternatives \
    --install   /usr/lib/llvm                       llvm                        "/usr/lib/llvm-$version"                        "$priority"     \
    --slave     /usr/bin/llvm-ar                    llvm-ar                     "/usr/bin/llvm-ar-$version"                                     \
    --slave     /usr/bin/llvm-as                    llvm-as                     "/usr/bin/llvm-as-$version"                                     \
    --slave     /usr/bin/llvm-bcanalyzer            llvm-bcanalyzer             "/usr/bin/llvm-bcanalyzer-$version"                             \
    --slave     /usr/bin/llvm-cat                   llvm-cat                    "/usr/bin/llvm-cat-$version"                                    \
    --slave     /usr/bin/llvm-cfi-verify            llvm-cfi-verify             "/usr/bin/llvm-cfi-verify-$version"                             \
    --slave     /usr/bin/llvm-config                llvm-config                 "/usr/bin/llvm-config-$version"                                 \
    --slave     /usr/bin/llvm-cov                   llvm-cov                    "/usr/bin/llvm-cov-$version"                                    \
    --slave     /usr/bin/llvm-c-test                llvm-c-test                 "/usr/bin/llvm-c-test-$version"                                 \
    --slave     /usr/bin/llvm-cvtres                llvm-cvtres                 "/usr/bin/llvm-cvtres-$version"                                 \
    --slave     /usr/bin/llvm-cxxdump               llvm-cxxdump                "/usr/bin/llvm-cxxdump-$version"                                \
    --slave     /usr/bin/llvm-cxxfilt               llvm-cxxfilt                "/usr/bin/llvm-cxxfilt-$version"                                \
    --slave     /usr/bin/llvm-cxxmap                llvm-cxxmap                 "/usr/bin/llvm-cxxmap-$version"                                 \
    --slave     /usr/bin/llvm-diff                  llvm-diff                   "/usr/bin/llvm-diff-$version"                                   \
    --slave     /usr/bin/llvm-dis                   llvm-dis                    "/usr/bin/llvm-dis-$version"                                    \
    --slave     /usr/bin/llvm-dlltool               llvm-dlltool                "/usr/bin/llvm-dlltool-$version"                                \
    --slave     /usr/bin/llvm-dwarfdump             llvm-dwarfdump              "/usr/bin/llvm-dwarfdump-$version"                              \
    --slave     /usr/bin/llvm-dwp                   llvm-dwp                    "/usr/bin/llvm-dwp-$version"                                    \
    --slave     /usr/bin/llvm-exegesis              llvm-exegesis               "/usr/bin/llvm-exegesis-$version"                               \
    --slave     /usr/bin/llvm-extract               llvm-extract                "/usr/bin/llvm-extract-$version"                                \
    --slave     /usr/bin/llvm-lib                   llvm-lib                    "/usr/bin/llvm-lib-$version"                                    \
    --slave     /usr/bin/llvm-link                  llvm-link                   "/usr/bin/llvm-link-$version"                                   \
    --slave     /usr/bin/llvm-lto2                  llvm-lto2                   "/usr/bin/llvm-lto2-$version"                                   \
    --slave     /usr/bin/llvm-lto                   llvm-lto                    "/usr/bin/llvm-lto-$version"                                    \
    --slave     /usr/bin/llvm-mc                    llvm-mc                     "/usr/bin/llvm-mc-$version"                                     \
    --slave     /usr/bin/llvm-mca                   llvm-mca                    "/usr/bin/llvm-mca-$version"                                    \
    --slave     /usr/bin/llvm-modextract            llvm-modextract             "/usr/bin/llvm-modextract-$version"                             \
    --slave     /usr/bin/llvm-mt                    llvm-mt                     "/usr/bin/llvm-mt-$version"                                     \
    --slave     /usr/bin/llvm-nm                    llvm-nm                     "/usr/bin/llvm-nm-$version"                                     \
    --slave     /usr/bin/llvm-objcopy               llvm-objcopy                "/usr/bin/llvm-objcopy-$version"                                \
    --slave     /usr/bin/llvm-objdump               llvm-objdump                "/usr/bin/llvm-objdump-$version"                                \
    --slave     /usr/bin/llvm-opt-report            llvm-opt-report             "/usr/bin/llvm-opt-report-$version"                             \
    --slave     /usr/bin/llvm-pdbutil               llvm-pdbutil                "/usr/bin/llvm-pdbutil-$version"                                \
    --slave     /usr/bin/llvm-PerfectShuffle        llvm-PerfectShuffle         "/usr/bin/llvm-PerfectShuffle-$version"                         \
    --slave     /usr/bin/llvm-profdata              llvm-profdata               "/usr/bin/llvm-profdata-$version"                               \
    --slave     /usr/bin/llvm-ranlib                llvm-ranlib                 "/usr/bin/llvm-ranlib-$version"                                 \
    --slave     /usr/bin/llvm-rc                    llvm-rc                     "/usr/bin/llvm-rc-$version"                                     \
    --slave     /usr/bin/llvm-readobj               llvm-readobj                "/usr/bin/llvm-readobj-$version"                                \
    --slave     /usr/bin/llvm-rtdyld                llvm-rtdyld                 "/usr/bin/llvm-rtdyld-$version"                                 \
    --slave     /usr/bin/llvm-size                  llvm-size                   "/usr/bin/llvm-size-$version"                                   \
    --slave     /usr/bin/llvm-split                 llvm-split                  "/usr/bin/llvm-split-$version"                                  \
    --slave     /usr/bin/llvm-stress                llvm-stress                 "/usr/bin/llvm-stress-$version"                                 \
    --slave     /usr/bin/llvm-strings               llvm-strings                "/usr/bin/llvm-strings-$version"                                \
    --slave     /usr/bin/llvm-strip                 llvm-strip                  "/usr/bin/llvm-strip-$version"                                  \
    --slave     /usr/bin/llvm-symbolizer            llvm-symbolizer             "/usr/bin/llvm-symbolizer-$version"                             \
    --slave     /usr/bin/llvm-tblgen                llvm-tblgen                 "/usr/bin/llvm-tblgen-$version"                                 \
    --slave     /usr/bin/llvm-undname               llvm-undname                "/usr/bin/llvm-undname-$version"                                \
    --slave     /usr/bin/llvm-xray                  llvm-xray                   "/usr/bin/llvm-xray-$version"

# clang
update-alternatives \
    --install   /usr/bin/clang                                  clang                       "/usr/bin/clang-$version"                       "$priority"     \
    --slave     /usr/bin/clang++                                clang++                     "/usr/bin/clang++-$version"                                     \
    --slave     /usr/bin/clang-apply-replacements               clang-apply-replacements    "/usr/bin/clang-apply-replacements-$version"                    \
    --slave     /usr/bin/clang-change-namespace                 clang-change-namespace      "/usr/bin/clang-change-namespace-$version"                      \
    --slave     /usr/bin/clang-check                            clang-check                 "/usr/bin/clang-check-$version"                                 \
    --slave     /usr/bin/clang-cl                               clang-cl                    "/usr/bin/clang-cl-$version"                                    \
    --slave     /usr/bin/clang-cpp                              clang-cpp                   "/usr/bin/clang-cpp-$version"                                   \
    --slave     /usr/bin/clangd                                 clangd                      "/usr/bin/clangd-$version"                                      \
    --slave     /usr/bin/clang-extdef-mapping                   clang-extdef-mapping        "/usr/bin/clang-extdef-mapping-$version"                        \
    --slave     /usr/bin/clang-format                           clang-format                "/usr/bin/clang-format-$version"                                \
    --slave     /usr/bin/clang-format-diff                      clang-format-diff           "/usr/bin/clang-format-diff-$version"                           \
    --slave     /usr/bin/clang-import-test                      clang-import-test           "/usr/bin/clang-import-test-$version"                           \
    --slave     /usr/bin/clang-include-fixer                    clang-include-fixer         "/usr/bin/clang-include-fixer-$version"                         \
    --slave     /usr/bin/clang-offload-bundler                  clang-offload-bundler       "/usr/bin/clang-offload-bundler-$version"                       \
    --slave     /usr/bin/clang-query                            clang-query                 "/usr/bin/clang-query-$version"                                 \
    --slave     /usr/bin/clang-refactor                         clang-refactor              "/usr/bin/clang-refactor-$version"                              \
    --slave     /usr/bin/clang-rename                           clang-rename                "/usr/bin/clang-rename-$version"                                \
    --slave     /usr/bin/clang-reorder-fields                   clang-reorder-fields        "/usr/bin/clang-reorder-fields-$version"                        \
    --slave     /usr/bin/scan-view                              scan-view                   "/usr/bin/scan-view-$version"                                   \
    --slave     /usr/bin/scan-build                             scan-build                  "/usr/bin/scan-build-$version"                                  \
    --slave     /usr/bin/scan-build-py                          scan-build-py               "/usr/bin/scan-build-py-$version"                               \
    --slave     /usr/bin/bugpoint                               bugpoint                    "/usr/bin/bugpoint-$version"                                    \
    --slave     /usr/bin/c-index-test                           c-index-test                "/usr/bin/c-index-test-$version"                                \
    --slave     /usr/bin/diagtool                               diagtool                    "/usr/bin/diagtool-$version"                                    \
    --slave     /usr/bin/find-all-symbols                       find-all-symbols            "/usr/bin/find-all-symbols-$version"                            \
    --slave     /usr/bin/git-clang-format                       git-clang-format            "/usr/bin/git-clang-format-$version"                            \
    --slave     /usr/bin/hmaptool                               hmaptool                    "/usr/bin/hmaptool-$version"                                    \
    --slave     /usr/bin/modularize                             modularize                  "/usr/bin/modularize-$version"                                  \
    --slave     /usr/bin/obj2yaml                               obj2yaml                    "/usr/bin/obj2yaml-$version"                                    \
    --slave     /usr/bin/opt                                    opt                         "/usr/bin/opt-$version"                                         \
    --slave     /usr/bin/sancov                                 sancov                      "/usr/bin/sancov-$version"                                      \
    --slave     /usr/bin/sanstats                               sanstats                    "/usr/bin/sanstats-$version"                                    \
    --slave     /usr/bin/verify-uselistorder                    verify-uselistorder         "/usr/bin/verify-uselistorder-$version"                         \
    --slave     /usr/bin/wasm-ld                                wasm-ld                     "/usr/bin/wasm-ld-$version"                                     \
    --slave     /usr/bin/yaml2obj                               yaml2obj                    "/usr/bin/yaml2obj-$version"                                    \
    --slave     /usr/bin/yaml-bench                             yaml-bench                  "/usr/bin/yaml-bench-$version"                                  \
    --slave     /usr/bin/lld                                    lld                         "/usr/bin/lld-$version"                                         \
    --slave     /usr/bin/lld-link                               lld-link                    "/usr/bin/lld-link-$version"                                    \
    --slave     /usr/bin/lli-child-target                       lli-child-target            "/usr/bin/lli-child-target-$version"                            \
    --slave     /usr/bin/lli                                    lli                         "/usr/bin/lli-$version"                                         \
    --slave     /usr/bin/llc                                    llc                         "/usr/bin/llc-$version"                                         \
    --slave     /usr/bin/lldb                                   lldb                        "/usr/bin/lldb-$version"                                        \
    --slave     /usr/bin/lldb-argdumper                         lldb-argdumper              "/usr/bin/lldb-argdumper-$version"                              \
    --slave     /usr/bin/lldb-mi                                lldb-mi                     "/usr/bin/lldb-mi-$version"                                     \
    --slave     /usr/bin/lldb-server                            lldb-server                 "/usr/bin/lldb-server-$version"                                 \
    --slave     /usr/bin/lldb-test                              lldb-test                   "/usr/bin/lldb-test-$version"                                   \
    --slave     /usr/bin/lldb-vscode                            lldb-vscode                 "/usr/bin/lldb-vscode-$version"                                 \
    --slave     /usr/share/man/man1/clang.1.gz                  clang.1.gz                  "/usr/share/man/man1/clang-$version.1.gz"                       \
    --slave     /usr/share/man/man1/clangd-1.gz                 clangd.1.gz                 "/usr/share/man/man1/clangd-$version.1.gz"                      \
    --slave     /usr/share/man/man1/clang-format.1.gz           clang-format.1.gz           "/usr/share/man/man1/clang-format-$version.1.gz"                \
    --slave     /usr/share/man/man1/clang-format-diff.1.gz      clang-format-diff.1.gz      "/usr/share/man/man1/clang-format-diff-$version.1.gz"

# make system default
update-alternatives --install /usr/bin/cc  cc  /usr/bin/clang      100
update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++    100
