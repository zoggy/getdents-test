(lang dune 3.7)

(name getdents)

(generate_opam_files true)

(authors "Zoggy <zoggy@bat8.org>")
(maintainers "zoggy@bat8.org")

(license LGPL-3.0-only)

(package
 (name getdents)
 (synopsis "Test program to use unix getdents with lwt")
 (description "Bla bla bla")
 (depends ocaml dune lwt lwt_ppx lwt.unix)
)

