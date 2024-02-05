
let main () =
  let dir = Sys.argv.(1) in
  let%lwt entries = Lwt_getdents.getdents dir in
  let%lwt entries = Lwt_getdents.getdents dir in
  let%lwt entries = Lwt_getdents.getdents dir in
  let%lwt entries = Lwt_getdents.getdents dir in
  let%lwt entries = Lwt_getdents.getdents dir in
  Gc.major();
  Lwt_list.iter_p Lwt_io.(write_line  stdout) entries

let () = Lwt_main.run (Lwt.join [main ();main ();main ()])

