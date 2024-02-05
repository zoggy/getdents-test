external getdents_job : string -> string list Lwt_unix.job = "lwt_unix_getdents_job"

let getdents dir = Lwt_unix.run_job (getdents_job dir)
