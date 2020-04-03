pipeline {
    agent any
    options {
      buildDiscarder(logRotator(daysToKeepStr: '90', artifactDaysToKeepStr: '90'))
    }
    
    post {
      failure {
        script {
           currentBuild.result = 'FAILED'
        }
      }
      success {
         script {
           currentBuild.result = 'SUCCESS'
        }
      }
    }
    
    stages {
        stage('Build') {
            parallel {
                stage('UnitTest') {
                    steps {
						bat 'cd toolchain/ceedling & ceedling.cmd gcov:all'
                        bat 'cd toolchain/ceedling & ceedling.cmd utils:gcov'
                        xunit([Custom(customXSL: 'toolchain/ceedling/unity.xsl', deleteOutputFiles: true, failIfNotNew: true, pattern: 'build/artifacts/gcov/report.xml', skipNoTestFiles: false, stopProcessingIfError: true)])
                        cobertura coberturaReportFile: 'build/artifacts/gcov/GcovCoverageResults.xml'
                    }
                }
                stage('CppCheck') {
                    steps {
                        bat '"C:/Program Files/Cppcheck/cppcheck.exe" -j4 --addon=misra --enable=all -I./src/ --xml --xml-version=2 ./src 2> cppcheck.xml'
                        publishCppcheck pattern:'cppcheck.xml'
                    }
                }
                stage('PClint')
                {
                    steps {
                        bat 'cd toolchain/pclint & "C:/Program Files (x86)/PC-lint 9.0/lint-nt.exe" jenkins.lnt & exit 0'
                        recordIssues qualityGates: [[threshold: 1, type: 'TOTAL', unstable: true]], tools: [pcLint(pattern: 'toolchain/pclint/lintResults.txt')]
                    }
                }
            }
        }
    }
}
