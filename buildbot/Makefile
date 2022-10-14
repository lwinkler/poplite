MASTER_DIR=masterdir
WORKER_DIR=workerdir

MASTERHOST=localhost
PORT=9989
WORKERNAME=poplite-worker
PASSWORD=pass

prepare:
	pip install -r requirements.txt

create-master:
	buildbot create-master -r ${MASTER_DIR}
	echo "Please copy master.cfg once to ${MASTER_DIR}"

create-worker:
	buildbot-worker create-worker ${WORKER_DIR} ${MASTERHOST}:${PORT} ${WORKERNAME} ${PASSWORD}

start-master:
	buildbot start ${MASTER_DIR}

stop-master:
	buildbot start ${MASTER_DIR}

start-worker:
	buildbot-worker start ${WORKER_DIR}

stop-worker:
	buildbot-worker stop ${WORKER_DIR}

update-master:
	buildbot reconfig ${MASTER_DIR}
