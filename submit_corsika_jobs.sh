#!/bin/bash

# Submits one SLURM job per CORSIKA input file found in INPUT_DIR.

# --- Configuration (edit these) ----------------------------------------------

# TODO: Is this the right path???
CORSIKA_EXE="$HOME/corsika-78050/run/corsika78050Linux_EPOS_urqmd"   # Full path to your CORSIKA binary
INPUT_DIR="/home/eu-physics3/EPIC-UI/build/corsika_inputs"         # Directory containing .inp files
INPUT_EXT="inp"                             # Input file extension

OUTPUT_DIR="../build/output"                # Directory for CORSIKA output files
LOG_DIR="../build/logs"                     # Directory for SLURM stdout/stderr logs


# TODO: Do we actually need these?
# SLURM settings
PARTITION="compute"                        # Partition/queue name
NTASKS=1                                    # CPUs per job
WALLTIME="04:00:00"                         # Max walltime (HH:MM:SS)

# -----------------------------------------------------------------------------

# Resolve absolute paths so jobs work regardless of working directory
INPUT_DIR="$(realpath "$INPUT_DIR")"
OUTPUT_DIR="$(realpath -m "$OUTPUT_DIR")"
LOG_DIR="$(realpath -m "$LOG_DIR")"

# Create output and log directories if they don't exist
mkdir -p "$OUTPUT_DIR" "$LOG_DIR"

# Check that the CORSIKA executable exists
if [[ ! -x "$CORSIKA_EXE" ]]; then
    echo "ERROR: CORSIKA executable not found or not executable: $CORSIKA_EXE"
    exit 1
fi

# Find all input files
INPUT_FILES=("$INPUT_DIR"/*."$INPUT_EXT")

if [[ ! -e "${INPUT_FILES[0]}" ]]; then
    echo "ERROR: No .$INPUT_EXT files found in $INPUT_DIR"
    exit 1
fi

echo "Found ${#INPUT_FILES[@]} input file(s). Submitting jobs..."

# Submit one job per input file
for INPUT_FILE in "${INPUT_FILES[@]}"; do

    BASENAME="$(basename "$INPUT_FILE" ."$INPUT_EXT")"  # e.g. "run_000001"
    LOG_OUT="$LOG_DIR/${BASENAME}.out"
    LOG_ERR="$LOG_DIR/${BASENAME}.err"

    JOB_SCRIPT=$(cat <<EOF
#!/bin/bash
#SBATCH --job-name=corsika_${BASENAME}
#SBATCH --partition=${PARTITION}
#SBATCH --ntasks=${NTASKS}
#SBATCH --time=${WALLTIME}
#SBATCH --output=${LOG_OUT}
#SBATCH --error=${LOG_ERR}

echo "Starting CORSIKA job: ${BASENAME}"
echo "Input file : ${INPUT_FILE}"
echo "Output dir : ${OUTPUT_DIR}"
echo "Started at : \$(date)"

cd "${OUTPUT_DIR}"

"${CORSIKA_EXE}" < "${INPUT_FILE}"

EXIT_CODE=\$?
echo "Finished at: \$(date)"
echo "Exit code  : \$EXIT_CODE"
exit \$EXIT_CODE
EOF
)

    # Submit the job and capture the job ID
    JOB_ID=$(echo "$JOB_SCRIPT" | sbatch --parsable)
    echo "  Submitted $BASENAME -> Job ID: $JOB_ID"

done

echo "Done. Check logs in: $LOG_DIR"
