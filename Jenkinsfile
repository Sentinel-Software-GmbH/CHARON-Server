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
						bat 'ceedling.cmd gcov:all'
                        bat 'ceedling.cmd utils:gcov'
                        xunit([Custom(customXSL: 'toolchain/ceedling/unity.xsl', deleteOutputFiles: true, failIfNotNew: true, pattern: 'build/artifacts/gcov/report.xml', skipNoTestFiles: false, stopProcessingIfError: true)])
                        cobertura coberturaReportFile: 'build/artifacts/gcov/GcovCoverageResults.xml'
                    }
                }
                stage('CppCheck') {
                    steps {
                        bat '"C:/Program Files/Cppcheck/cppcheck.exe" -j4 --enable=all -I./src/Library --xml --xml-version=2 ./src/Library 2> cppcheck.xml'
                        publishCppcheck pattern:'cppcheck.xml'
                    }
                }
                stage('PClint')
                {
                    steps {
                        bat 'cd toolchain/pclint & "C:/Program Files (x86)/PC-lint 9.0/lint-nt.exe" jenkins.lnt & exit 0'
                        recordIssues tools: [pcLint(pattern: 'toolchain/pclint/lintResults.txt')]
                    }
                }
                stage('Windows Port') {
                    steps {
                        bat 'cd src/Port/Windows & ceedling.cmd release'
                    }
                }
                stage('STM32F4 Port') {
                    steps {
                        bat 'cd src/Port/STM32F4-Discovery & ceedling.cmd release'
                    }
                }
            }
        }
        stage('check warnings') {
            steps {
                recordIssues qualityGates: [[threshold: 1, type: 'TOTAL', unstable: false]], tools:[gcc()]
            }
        }
    }
}
