#!/bin/bash
# Submits one SLURM job per CORSIKA input file found in INPUT_DIR.
# Works across all nodes — no shared filesystem required.
# Logs and results are written locally on the worker, then scp'd back.

# --- Configuration (edit these) ----------------------------------------------
CORSIKA_RELATIVE="Downloads/corsika-78010/run/corsika78010Linux_EPOS_urqmd"
CORSIKA_RUN_RELATIVE="Downloads/corsika-78010/run"

INPUT_DIR="/home/eu-physics3/EPIC-UI/build/corsika_inputs"
INPUT_EXT="steer"

# Controller (this machine) — where results and logs are collected
CONTROLLER_USER="eu-physics3"
CONTROLLER_HOST="eu-physics3-Macmini5-2"
CONTROLLER_OUTPUT_DIR="/home/eu-physics3/EPIC-UI/build/output"
CONTROLLER_LOG_DIR="/home/eu-physics3/EPIC-UI/build/logs"

# SLURM settings
NTASKS=1
WALLTIME="04:00:00"
PARTITION="compute"
# -----------------------------------------------------------------------------

mkdir -p "$CONTROLLER_OUTPUT_DIR" "$CONTROLLER_LOG_DIR"

INPUT_FILES=("$INPUT_DIR"/*."$INPUT_EXT")
if [[ ! -e "${INPUT_FILES[0]}" ]]; then
    echo "ERROR: No .$INPUT_EXT files found in $INPUT_DIR"
    exit 1
fi

echo "Found ${#INPUT_FILES[@]} input file(s). Submitting jobs..."

for INPUT_FILE in "${INPUT_FILES[@]}"; do
    BASENAME="$(basename "$INPUT_FILE" ."$INPUT_EXT")"
    RUNNR=$(grep -i "^RUNNR" "$INPUT_FILE" | awk '{print $2}')
    DAT_FILENAME="$(printf "DAT%06d" "$RUNNR")"
    STEER_CONTENT="$(cat "$INPUT_FILE")"

    JOB_SCRIPT=$(cat <<SBATCH
#!/bin/bash
#SBATCH --job-name=corsika_${BASENAME}
#SBATCH --partition=${PARTITION}
#SBATCH --ntasks=${NTASKS}
#SBATCH --time=${WALLTIME}
#SBATCH --output=/tmp/corsika_${BASENAME}_slurm.out
#SBATCH --error=/tmp/corsika_${BASENAME}_slurm.err

echo "Starting CORSIKA job: ${BASENAME}"
echo "Running on node: \$(hostname)"
echo "Started at: \$(date)"

# Per-node home directory lookup — add new nodes here as needed
declare -A NODE_HOME
NODE_HOME["OWTestStation"]="/home/halliday"
NODE_HOME["big-data-1-System-Product-Name"]="/home/big-data-1"
NODE_HOME["eu-physics-fast-MS-7E06"]="/home/eu-physics-fast"
NODE_HOME["fred-fuchs-To-Be-Filled-By-O-E-M"]="/home/fred-fuchs"

NODE_HOSTNAME="\$(hostname)"
WORKER_HOME="\${NODE_HOME[\$NODE_HOSTNAME]}"

if [[ -z "\$WORKER_HOME" ]]; then
    echo "ERROR: No home directory mapping found for node \$NODE_HOSTNAME"
    echo "Add it to the NODE_HOME lookup table in submit_corsika_jobs.sh"
    scp /tmp/corsika_${BASENAME}_slurm.out ${CONTROLLER_USER}@${CONTROLLER_HOST}:${CONTROLLER_LOG_DIR}/${BASENAME}.out 2>/dev/null || true
    exit 1
fi

CORSIKA_EXE="\$WORKER_HOME/${CORSIKA_RELATIVE}"
CORSIKA_RUN_DIR="\$WORKER_HOME/${CORSIKA_RUN_RELATIVE}"

if [[ ! -x "\$CORSIKA_EXE" ]]; then
    echo "ERROR: CORSIKA not found at \$CORSIKA_EXE"
    scp /tmp/corsika_${BASENAME}_slurm.out ${CONTROLLER_USER}@${CONTROLLER_HOST}:${CONTROLLER_LOG_DIR}/${BASENAME}.out 2>/dev/null || true
    exit 1
fi

JOB_SCRATCH="\$WORKER_HOME/corsika_scratch/${BASENAME}"
mkdir -p "\$JOB_SCRATCH"

# Write steer file, override DIRECT to local scratch
cat > "\$JOB_SCRATCH/run.steer" << 'STEER'
${STEER_CONTENT}
STEER
sed -i "s|^DIRECT.*|DIRECT  \$JOB_SCRATCH/|" "\$JOB_SCRATCH/run.steer"

rm -f "\$JOB_SCRATCH/${DAT_FILENAME}"
rm -f "\$JOB_SCRATCH/${DAT_FILENAME}.long"

cd "\$CORSIKA_RUN_DIR"
"\$CORSIKA_EXE" < "\$JOB_SCRATCH/run.steer" > "\$JOB_SCRATCH/corsika.log" 2>&1
EXIT_CODE=\$?

echo "CORSIKA exit code: \$EXIT_CODE"
cat "\$JOB_SCRATCH/corsika.log"

# Always scp logs back
scp /tmp/corsika_${BASENAME}_slurm.out ${CONTROLLER_USER}@${CONTROLLER_HOST}:${CONTROLLER_LOG_DIR}/${BASENAME}.out 2>/dev/null || true
scp /tmp/corsika_${BASENAME}_slurm.err ${CONTROLLER_USER}@${CONTROLLER_HOST}:${CONTROLLER_LOG_DIR}/${BASENAME}.err 2>/dev/null || true
scp "\$JOB_SCRATCH/corsika.log" ${CONTROLLER_USER}@${CONTROLLER_HOST}:${CONTROLLER_LOG_DIR}/${BASENAME}.corsika.log 2>/dev/null || true

if [[ \$EXIT_CODE -eq 0 ]]; then
    echo "Copying results back to controller..."
    scp "\$JOB_SCRATCH/${DAT_FILENAME}" ${CONTROLLER_USER}@${CONTROLLER_HOST}:${CONTROLLER_OUTPUT_DIR}/ 2>/dev/null || true
    scp "\$JOB_SCRATCH/${DAT_FILENAME}.long" ${CONTROLLER_USER}@${CONTROLLER_HOST}:${CONTROLLER_OUTPUT_DIR}/ 2>/dev/null || true
    echo "Results copied successfully."
else
    echo "CORSIKA failed — check ${BASENAME}.corsika.log"
fi

rm -rf "\$JOB_SCRATCH"

echo "Finished at: \$(date)"
exit \$EXIT_CODE
SBATCH
)

    JOB_ID=$(echo "$JOB_SCRIPT" | sbatch --parsable)
    echo "  Submitted $BASENAME -> Job ID: $JOB_ID"
done

echo "Done. Check logs in: $CONTROLLER_LOG_DIR"
